/*
    CONTRIBUTORS:
        Sean Pesce

    Undocumented function prototypes were determined using IDA Pro and the following references:
        https://github.com/doitsujin/dxvk/blob/master/src/d3d11/d3d11_main.cpp
        https://github.com/bo3b/3Dmigoto/blob/master/DirectX11/D3D11Wrapper.cpp
        https://chromium.googlesource.com/external/p3/regal/+/cass/src/apitrace/wrappers/d3d11stubs.cpp
*/

#include "d3d9/exports.h"

HRESULT WINAPI Ordinal_16_()
{
    HRESULT result = Ordinal_16_t(d3d9::functions[d3d9::Ordinal_16_i])();
	return result;
}

HRESULT WINAPI Ordinal_17_()
{
    HRESULT result = Ordinal_17_t(d3d9::functions[d3d9::Ordinal_17_i])();
    return result;
}

HRESULT WINAPI Ordinal_18_()
{
    HRESULT result = Ordinal_18_t(d3d9::functions[d3d9::Ordinal_18_i])();
    return result;
}

HRESULT WINAPI Ordinal_19_()
{
    HRESULT result = Ordinal_19_t(d3d9::functions[d3d9::Ordinal_19_i])();
    return result;
}

IDirect3D9* WINAPI Direct3DCreate9On12_(_In_ UINT sdk_version, _In_ /*D3D9ON12_ARGS*/VOID* override_list, _In_ UINT override_entries)
{
    IDirect3D9* result = Direct3DCreate9On12_t(d3d9::functions[d3d9::Direct3DCreate9On12_i])(sdk_version, override_list, override_entries);
    return result;
}

IDirect3D9* WINAPI Direct3DCreate9On12Ex_(_In_ UINT sdk_version, _In_ /*D3D9ON12_ARGS*/VOID* override_list, _In_ UINT override_entries)
{
    IDirect3D9* result = Direct3DCreate9On12Ex_t(d3d9::functions[d3d9::Direct3DCreate9On12Ex_i])(sdk_version, override_list, override_entries);
    return result;
}

HRESULT WINAPI Ordinal_22_()
{
    HRESULT result = Ordinal_22_t(d3d9::functions[d3d9::Ordinal_22_i])();
    return result;
}

HRESULT WINAPI Ordinal_23_()
{
    HRESULT result = Ordinal_23_t(d3d9::functions[d3d9::Ordinal_23_i])();
    return result;
}

HRESULT WINAPI Direct3DShaderValidatorCreate9_()
{
    HRESULT result = Direct3DShaderValidatorCreate9_t(d3d9::functions[d3d9::Direct3DShaderValidatorCreate9_i])();
    return result;
}

HRESULT WINAPI PSGPError_()
{
    HRESULT result = PSGPError_t(d3d9::functions[d3d9::PSGPError_i])();
    return result;
}

HRESULT WINAPI PSGPSampleTexture_()
{
    HRESULT result = PSGPSampleTexture_t(d3d9::functions[d3d9::PSGPSampleTexture_i])();
    return result;
}

HRESULT WINAPI D3DPERF_BeginEvent_(_In_ D3DCOLOR col, _In_ LPCWSTR wszName)
{
    HRESULT result = D3DPERF_BeginEvent_t(d3d9::functions[d3d9::D3DPERF_BeginEvent_i])(col, wszName);
    return result;
}

HRESULT WINAPI D3DPERF_EndEvent_()
{
    HRESULT result = D3DPERF_EndEvent_t(d3d9::functions[d3d9::D3DPERF_EndEvent_i])();
    return result;
}

DWORD WINAPI D3DPERF_GetStatus_()
{
    DWORD result = D3DPERF_GetStatus_t(d3d9::functions[d3d9::D3DPERF_GetStatus_i])();
    return result;
}

BOOL WINAPI D3DPERF_QueryRepeatFrame_()
{
    BOOL result = D3DPERF_QueryRepeatFrame_t(d3d9::functions[d3d9::D3DPERF_QueryRepeatFrame_i])();
    return result;
}

HRESULT WINAPI D3DPERF_SetMarker_(_In_ D3DCOLOR col, _In_ LPCWSTR wszName)
{
    HRESULT result = D3DPERF_SetMarker_t(d3d9::functions[d3d9::D3DPERF_SetMarker_i])(col, wszName);
    return result;
}

HRESULT WINAPI D3DPERF_SetOptions_(_In_ DWORD options)
{
    HRESULT result = D3DPERF_SetOptions_t(d3d9::functions[d3d9::D3DPERF_SetOptions_i])(options);
    return result;
}

HRESULT WINAPI D3DPERF_SetRegion_(_In_ D3DCOLOR col, _In_ LPCWSTR wszName)
{
    HRESULT result = D3DPERF_SetRegion_t(d3d9::functions[d3d9::D3DPERF_SetRegion_i])(col, wszName);
    return result;
}

HRESULT WINAPI DebugSetLevel_(_In_ DWORD level)
{
    HRESULT result = DebugSetLevel_t(d3d9::functions[d3d9::DebugSetLevel_i])(level);
    return result;
}

HRESULT WINAPI DebugSetMute_(_In_ BOOL mute)
{
    HRESULT result = DebugSetMute_t(d3d9::functions[d3d9::DebugSetMute_i])(mute);
    return result;
}

HRESULT WINAPI Direct3D9EnableMaximizedWindowedModeShim_(_In_ int int1)
{
    HRESULT result = Direct3D9EnableMaximizedWindowedModeShim_t(d3d9::functions[d3d9::Direct3D9EnableMaximizedWindowedModeShim_i])(int1);
	return result;
}

IDirect3D9* WINAPI Direct3DCreate9_(_In_ UINT sdk_version)
{
    IDirect3D9* result = Direct3DCreate9_t(d3d9::functions[d3d9::Direct3DCreate9_i])(sdk_version);
    return result;
}

HRESULT WINAPI Direct3DCreate9Ex_(_In_ UINT sdk_version, _Out_ IDirect3D9Ex** pp)
{
    HRESULT result = Direct3DCreate9Ex_t(d3d9::functions[d3d9::Direct3DCreate9Ex_i])(sdk_version, pp);
    return result;
}
