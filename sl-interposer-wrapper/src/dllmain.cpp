// Diagnostic / redirect proxy for NVIDIA Streamline's sl.interposer.dll
//
// Purpose: the game (yysls.exe) links against sl.interposer.dll's re-exported
// D3D12/DXGI entry points. Streamline internally reaches the real D3D12/DXGI by
// manually walking the export table, which bypasses RenderDoc's IAT and
// GetProcAddress hooks (see RenderDoc issue #3257). By dropping this proxy in
// place of sl.interposer.dll we take control of those entry points ourselves.
//
// The 14 D3D/DXGI exports are implemented here as real C stubs that:
//   1. log the call (so we can see exactly what the game invokes), and
//   2. forward to the REAL d3d11/d3d12/dxgi system DLLs via GetProcAddress
//      (NOT to the real interposer). Because we call the real API through a
//      normal GetProcAddress-resolved pointer, RenderDoc's existing hooks on
//      d3d12core.dll / dxgi.dll fire normally -> device + swapchain get wrapped
//      -> F12 capture works.
//
// The 19 sl* and 229 vk* exports are forwarded (via /export linker directives in
// forwards.h) to the renamed real interposer (slinterposerorig.dll -- dot-free
// name; MSVC linker mis-parses dotted forwarder module names) so Streamline's own
// API keeps working for anything that still calls it. (Diagnostic build:
// DLSS/Reflex may be degraded because we bypass the interposer for D3D12 device
// creation; that's fine for capture.)
//
// INSTALL:
//   1. rename real  Streamline\sl.interposer.dll  ->  slinterposerorig.dll
//   2. drop this built DLL in as                       sl.interposer.dll
//
#include <windows.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdio>
#include <cstdarg>

// pragma-based forwarders for the 248 sl*/vk* exports -> slinterposerorig.<name>
// (MSVC .def does not support name=module.func forwarders; /export directives do)
#include "forwards.h"

// ------------------------------------------------------------------ logging
static FILE *g_log = NULL;
static CRITICAL_SECTION g_cs;
static LONG g_callSeq = 0;

static void wlog(const char *fmt, ...)
{
  if(!g_log)
    return;
  EnterCriticalSection(&g_cs);
  SYSTEMTIME st;
  GetLocalTime(&st);
  fprintf(g_log, "[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
  va_list ap;
  va_start(ap, fmt);
  vfprintf(g_log, fmt, ap);
  va_end(ap);
  fprintf(g_log, "\n");
  fflush(g_log);
  LeaveCriticalSection(&g_cs);
}

// ------------------------------------------------- real system DLL resolving
static HMODULE g_d3d11 = NULL;
static HMODULE g_d3d12 = NULL;
static HMODULE g_dxgi = NULL;
static HMODULE g_realInterposer = NULL;    // sl.interposer.orig.dll (for reference/fallback)

static HMODULE loadSystem(const char *name)
{
  char path[MAX_PATH] = {};
  UINT n = GetSystemDirectoryA(path, MAX_PATH);
  if(n == 0 || n >= MAX_PATH)
    return LoadLibraryA(name);
  strcat_s(path, "\\");
  strcat_s(path, name);
  HMODULE m = LoadLibraryA(path);
  wlog("  loadSystem(%s) -> %p", path, m);
  return m;
}

static FARPROC realProc(HMODULE mod, const char *modname, const char *fn)
{
  FARPROC p = mod ? GetProcAddress(mod, fn) : NULL;
  if(!p)
    wlog("  !! GetProcAddress(%s, %s) FAILED", modname, fn);
  return p;
}

// generic "log + forward to real system dll" helper for the D3D/DXGI exports.
// We resolve the real function lazily and cache it.
#define FORWARD_BODY(MOD, MODNAME, FN, SIG, ...)                                   \
  do                                                                               \
  {                                                                                \
    static SIG s_real = NULL;                                                      \
    LONG seq = InterlockedIncrement(&g_callSeq);                                   \
    wlog("[FAKESL #%ld] %s -> forwarding to %s!%s", seq, FN, MODNAME, FN);         \
    if(!s_real)                                                                    \
      s_real = (SIG)realProc(MOD, MODNAME, FN);                                    \
    if(!s_real)                                                                    \
      return E_FAIL;                                                               \
    return s_real(__VA_ARGS__);                                                    \
  } while(0)

extern "C" {

// ---- DXGI ----
HRESULT WINAPI w_CreateDXGIFactory(REFIID riid, void **pp)
{
  typedef HRESULT(WINAPI * FN)(REFIID, void **);
  FORWARD_BODY(g_dxgi, "dxgi.dll", "CreateDXGIFactory", FN, riid, pp);
}
HRESULT WINAPI w_CreateDXGIFactory1(REFIID riid, void **pp)
{
  typedef HRESULT(WINAPI * FN)(REFIID, void **);
  FORWARD_BODY(g_dxgi, "dxgi.dll", "CreateDXGIFactory1", FN, riid, pp);
}
HRESULT WINAPI w_CreateDXGIFactory2(UINT flags, REFIID riid, void **pp)
{
  typedef HRESULT(WINAPI * FN)(UINT, REFIID, void **);
  FORWARD_BODY(g_dxgi, "dxgi.dll", "CreateDXGIFactory2", FN, flags, riid, pp);
}
HRESULT WINAPI w_DXGIGetDebugInterface1(UINT flags, REFIID riid, void **pp)
{
  typedef HRESULT(WINAPI * FN)(UINT, REFIID, void **);
  FORWARD_BODY(g_dxgi, "dxgi.dll", "DXGIGetDebugInterface1", FN, flags, riid, pp);
}

// ---- D3D11 ----
HRESULT WINAPI w_D3D11CreateDevice(IDXGIAdapter *a, D3D_DRIVER_TYPE dt, HMODULE sw, UINT flags,
                                   const D3D_FEATURE_LEVEL *fl, UINT nfl, UINT sdk,
                                   ID3D11Device **dev, D3D_FEATURE_LEVEL *ofl,
                                   ID3D11DeviceContext **ctx)
{
  typedef HRESULT(WINAPI * FN)(IDXGIAdapter *, D3D_DRIVER_TYPE, HMODULE, UINT,
                               const D3D_FEATURE_LEVEL *, UINT, UINT, ID3D11Device **,
                               D3D_FEATURE_LEVEL *, ID3D11DeviceContext **);
  FORWARD_BODY(g_d3d11, "d3d11.dll", "D3D11CreateDevice", FN, a, dt, sw, flags, fl, nfl, sdk, dev,
               ofl, ctx);
}
HRESULT WINAPI w_D3D11CreateDeviceAndSwapChain(IDXGIAdapter *a, D3D_DRIVER_TYPE dt, HMODULE sw,
                                               UINT flags, const D3D_FEATURE_LEVEL *fl, UINT nfl,
                                               UINT sdk, const DXGI_SWAP_CHAIN_DESC *scd,
                                               IDXGISwapChain **sc, ID3D11Device **dev,
                                               D3D_FEATURE_LEVEL *ofl, ID3D11DeviceContext **ctx)
{
  typedef HRESULT(WINAPI * FN)(IDXGIAdapter *, D3D_DRIVER_TYPE, HMODULE, UINT,
                               const D3D_FEATURE_LEVEL *, UINT, UINT, const DXGI_SWAP_CHAIN_DESC *,
                               IDXGISwapChain **, ID3D11Device **, D3D_FEATURE_LEVEL *,
                               ID3D11DeviceContext **);
  FORWARD_BODY(g_d3d11, "d3d11.dll", "D3D11CreateDeviceAndSwapChain", FN, a, dt, sw, flags, fl, nfl,
               sdk, scd, sc, dev, ofl, ctx);
}

// ---- D3D12 ----
HRESULT WINAPI w_D3D12CreateDevice(IUnknown *adapter, D3D_FEATURE_LEVEL fl, REFIID riid, void **dev)
{
  typedef HRESULT(WINAPI * FN)(IUnknown *, D3D_FEATURE_LEVEL, REFIID, void **);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12CreateDevice", FN, adapter, fl, riid, dev);
}
HRESULT WINAPI w_D3D12GetDebugInterface(REFIID riid, void **pp)
{
  typedef HRESULT(WINAPI * FN)(REFIID, void **);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12GetDebugInterface", FN, riid, pp);
}
HRESULT WINAPI w_D3D12GetInterface(REFCLSID clsid, REFIID riid, void **pp)
{
  typedef HRESULT(WINAPI * FN)(REFCLSID, REFIID, void **);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12GetInterface", FN, clsid, riid, pp);
}
HRESULT WINAPI w_D3D12EnableExperimentalFeatures(UINT n, const IID *iids, void *cfgs, UINT *sizes)
{
  typedef HRESULT(WINAPI * FN)(UINT, const IID *, void *, UINT *);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12EnableExperimentalFeatures", FN, n, iids, cfgs, sizes);
}
HRESULT WINAPI w_D3D12SerializeRootSignature(const D3D12_ROOT_SIGNATURE_DESC *desc,
                                             D3D_ROOT_SIGNATURE_VERSION ver, ID3DBlob **blob,
                                             ID3DBlob **err)
{
  typedef HRESULT(WINAPI * FN)(const D3D12_ROOT_SIGNATURE_DESC *, D3D_ROOT_SIGNATURE_VERSION,
                               ID3DBlob **, ID3DBlob **);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12SerializeRootSignature", FN, desc, ver, blob, err);
}
HRESULT WINAPI w_D3D12SerializeVersionedRootSignature(
    const D3D12_VERSIONED_ROOT_SIGNATURE_DESC *desc, ID3DBlob **blob, ID3DBlob **err)
{
  typedef HRESULT(WINAPI * FN)(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC *, ID3DBlob **,
                               ID3DBlob **);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12SerializeVersionedRootSignature", FN, desc, blob, err);
}
HRESULT WINAPI w_D3D12CreateRootSignatureDeserializer(LPCVOID data, SIZE_T size, REFIID riid,
                                                      void **pp)
{
  typedef HRESULT(WINAPI * FN)(LPCVOID, SIZE_T, REFIID, void **);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12CreateRootSignatureDeserializer", FN, data, size, riid,
               pp);
}
HRESULT WINAPI w_D3D12CreateVersionedRootSignatureDeserializer(LPCVOID data, SIZE_T size,
                                                               REFIID riid, void **pp)
{
  typedef HRESULT(WINAPI * FN)(LPCVOID, SIZE_T, REFIID, void **);
  FORWARD_BODY(g_d3d12, "d3d12.dll", "D3D12CreateVersionedRootSignatureDeserializer", FN, data,
               size, riid, pp);
}

}  // extern "C"

// ------------------------------------------------------------------ DllMain
static void init(HMODULE self)
{
  InitializeCriticalSection(&g_cs);

  // log next to the DLL
  char dir[MAX_PATH] = {};
  GetModuleFileNameA(self, dir, MAX_PATH);
  char *slash = strrchr(dir, '\\');
  if(slash)
    *slash = 0;
  char logpath[MAX_PATH] = {};
  sprintf_s(logpath, "%s\\fakesl.log", dir);
  fopen_s(&g_log, logpath, "w");

  wlog("=== FAKE sl.interposer.dll loaded (diagnostic proxy) ===");
  wlog("self dir: %s", dir);

  // load real system DLLs for the D3D/DXGI forwards. These go through normal
  // LoadLibrary/GetProcAddress so RenderDoc (already injected) hooks them.
  g_d3d11 = loadSystem("d3d11.dll");
  g_d3d12 = loadSystem("d3d12.dll");
  g_dxgi = loadSystem("dxgi.dll");

  // the sl*/vk* forwarders (in forwards.h) reference slinterposerorig.dll by
  // name (dot-free module name -- MSVC linker mis-parses dotted forwarder module
  // names). The loader will pull it in. We also LoadLibrary it explicitly so we
  // can log success and so it's present before any forwarded call.
  char orig[MAX_PATH] = {};
  sprintf_s(orig, "%s\\slinterposerorig.dll", dir);
  g_realInterposer = LoadLibraryA(orig);
  wlog("real interposer (slinterposerorig.dll) -> %p", g_realInterposer);

  // pull RenderDoc into the process (same trick as the d3d11/d3d9 wrappers).
  HMODULE rdoc = LoadLibraryA("senderdod.dll");
  wlog("senderdod.dll -> %p", rdoc);
  if(!rdoc)
    wlog("  (senderdod.dll not found on search path; RenderDoc injection may already be present)");

  wlog("=== init done ===");
}

BOOL APIENTRY DllMain(HMODULE self, DWORD reason, LPVOID reserved)
{
  if(reason == DLL_PROCESS_ATTACH)
  {
    DisableThreadLibraryCalls(self);
    init(self);
  }
  return TRUE;
}
