/*
    CONTRIBUTORS:
        Sean Pesce

*/
#pragma once

#ifndef SP_D3D9_DLL_DATA_MAIN_H_
#define SP_D3D9_DLL_DATA_MAIN_H_

//#include "D3D11.h"
//#include "Windows.Graphics.DirectX.Direct3D11.interop.h"
#include "D3D9.h"

namespace d3d9 {

constexpr const char* filename = "d3d9.dll";
const size_t func_count = 23; // func_count = ? on Windows 7
enum func_index {
    Ordinal_16_i, Ordinal_17_i, Ordinal_18_i, Ordinal_19_i, Direct3DCreate9On12_i,
    Direct3DCreate9On12Ex_i, Ordinal_22_i, Ordinal_23_i, Direct3DShaderValidatorCreate9_i, PSGPError_i,
    PSGPSampleTexture_i, D3DPERF_BeginEvent_i, D3DPERF_EndEvent_i, D3DPERF_GetStatus_i, D3DPERF_QueryRepeatFrame_i,
    D3DPERF_SetMarker_i, D3DPERF_SetOptions_i, D3DPERF_SetRegion_i, DebugSetLevel_i, DebugSetMute_i,
    Direct3D9EnableMaximizedWindowedModeShim_i, Direct3DCreate9_i, Direct3DCreate9Ex_i
};
constexpr const char* func_names[func_count] = {
    "Ordinal_16", "Ordinal_17", "Ordinal_18", "Ordinal_19", "Direct3DCreate9On12",
    "Direct3DCreate9On12Ex", "Ordinal_22", "Ordinal_23", "Direct3DShaderValidatorCreate9", "PSGPError",
    "PSGPSampleTexture", "D3DPERF_BeginEvent", "D3DPERF_EndEvent", "D3DPERF_GetStatus", "D3DPERF_QueryRepeatFrame",
    "D3DPERF_SetMarker", "D3DPERF_SetOptions", "D3DPERF_SetRegion", "DebugSetLevel", "DebugSetMute",
    "Direct3D9EnableMaximizedWindowedModeShim", "Direct3DCreate9", "Direct3DCreate9Ex"
};

extern HMODULE chain;
extern FARPROC functions[func_count];

void hook_exports();

} // namespace d3d9

#endif // SP_D3D9_DLL_DATA_MAIN_H_