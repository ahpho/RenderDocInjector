/*
    CONTRIBUTORS:
        Sean Pesce

    Undocumented function prototypes were determined using IDA Pro and the following references:
        https://github.com/doitsujin/dxvk/blob/master/src/d3d11/d3d11_main.cpp
        https://github.com/bo3b/3Dmigoto/blob/master/DirectX11/D3D11Wrapper.cpp
        https://chromium.googlesource.com/external/p3/regal/+/cass/src/apitrace/wrappers/d3d11stubs.cpp
*/
#pragma once

#ifndef SP_D3D9_DLL_EXPORTS_H_
#define SP_D3D9_DLL_EXPORTS_H_

#include "main.h"


typedef HRESULT (WINAPI * Ordinal_16_t)();
HRESULT WINAPI Ordinal_16_();

typedef HRESULT (WINAPI* Ordinal_17_t)();
HRESULT WINAPI Ordinal_17_();

typedef HRESULT (WINAPI* Ordinal_18_t)();
HRESULT WINAPI Ordinal_18_();

typedef HRESULT (WINAPI* Ordinal_19_t)();
HRESULT WINAPI Ordinal_19_();

typedef IDirect3D9* (WINAPI* Direct3DCreate9On12_t)(UINT sdk_version, /*D3D9ON12_ARGS*/VOID* override_list, UINT override_entries);
IDirect3D9* WINAPI Direct3DCreate9On12_(_In_ UINT sdk_version, _In_ /*D3D9ON12_ARGS*/VOID* override_list, _In_ UINT override_entries);

typedef IDirect3D9* (WINAPI* Direct3DCreate9On12Ex_t)(UINT sdk_version, /*D3D9ON12_ARGS*/VOID* override_list, UINT override_entries);
IDirect3D9* WINAPI Direct3DCreate9On12Ex_(_In_ UINT sdk_version, _In_ /*D3D9ON12_ARGS*/VOID* override_list, _In_ UINT override_entries);

typedef HRESULT (WINAPI* Ordinal_22_t)();
HRESULT WINAPI Ordinal_22_();

typedef HRESULT (WINAPI* Ordinal_23_t)();
HRESULT WINAPI Ordinal_23_();

typedef HRESULT (WINAPI* Direct3DShaderValidatorCreate9_t)();
HRESULT WINAPI Direct3DShaderValidatorCreate9_();

typedef HRESULT (WINAPI* PSGPError_t)();
HRESULT WINAPI PSGPError_();

typedef HRESULT (WINAPI* PSGPSampleTexture_t)();
HRESULT WINAPI PSGPSampleTexture_();

typedef HRESULT (WINAPI* D3DPERF_BeginEvent_t)(D3DCOLOR col, LPCWSTR wszName);
HRESULT WINAPI D3DPERF_BeginEvent_(_In_ D3DCOLOR col, _In_ LPCWSTR wszName);

typedef HRESULT (WINAPI* D3DPERF_EndEvent_t)();
HRESULT WINAPI D3DPERF_EndEvent_();

typedef DWORD (WINAPI* D3DPERF_GetStatus_t)();
DWORD WINAPI D3DPERF_GetStatus_();

typedef BOOL (WINAPI* D3DPERF_QueryRepeatFrame_t)();
BOOL WINAPI D3DPERF_QueryRepeatFrame_();

typedef HRESULT(WINAPI* D3DPERF_SetMarker_t)(D3DCOLOR col, LPCWSTR wszName);
HRESULT WINAPI D3DPERF_SetMarker_(_In_ D3DCOLOR col, _In_ LPCWSTR wszName);

typedef HRESULT(WINAPI* D3DPERF_SetOptions_t)(DWORD dwOptions);
HRESULT WINAPI D3DPERF_SetOptions_(_In_ DWORD dwOptions);

typedef HRESULT (WINAPI* D3DPERF_SetRegion_t)(D3DCOLOR col, LPCWSTR wszName);
HRESULT WINAPI D3DPERF_SetRegion_(_In_ D3DCOLOR col, _In_ LPCWSTR wszName);

typedef HRESULT (WINAPI* DebugSetLevel_t)(DWORD dw1);
HRESULT WINAPI DebugSetLevel_(_In_ DWORD dw1);

typedef HRESULT (WINAPI* DebugSetMute_t)(DWORD dw1);
HRESULT WINAPI DebugSetMute_(_In_ DWORD dw1);

typedef HRESULT(WINAPI* Direct3D9EnableMaximizedWindowedModeShim_t)(int int1);
HRESULT WINAPI Direct3D9EnableMaximizedWindowedModeShim_(_In_ int int1);

typedef IDirect3D9* (WINAPI* Direct3DCreate9_t)(UINT sdk_version);
IDirect3D9* WINAPI Direct3DCreate9_(_In_ UINT sdk_version);

typedef HRESULT (WINAPI* Direct3DCreate9Ex_t)(UINT sdk_version, IDirect3D9Ex** pp);
HRESULT WINAPI Direct3DCreate9Ex_(_In_ UINT sdk_version, _Out_ IDirect3D9Ex** pp);

#endif // SP_D3D9_DLL_EXPORTS_H_
