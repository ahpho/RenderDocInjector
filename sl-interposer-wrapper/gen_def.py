#!/usr/bin/env python3
"""Generate exports.def for the sl.interposer.dll diagnostic proxy.

  - 14 D3D/DXGI exports  -> our own C stubs (w_<name>) so we can log + redirect.
  - 19 sl* + 229 vk* exports -> loader forwarders to the renamed real interposer
    (sl.interposer.orig.dll). Zero code, always ABI-correct.
"""
import os

D3D = [
    "CreateDXGIFactory","CreateDXGIFactory1","CreateDXGIFactory2",
    "D3D11CreateDevice","D3D11CreateDeviceAndSwapChain",
    "D3D12CreateDevice","D3D12CreateRootSignatureDeserializer",
    "D3D12CreateVersionedRootSignatureDeserializer","D3D12EnableExperimentalFeatures",
    "D3D12GetDebugInterface","D3D12GetInterface",
    "D3D12SerializeRootSignature","D3D12SerializeVersionedRootSignature",
    "DXGIGetDebugInterface1",
]
SL = [
    "slAllocateResources","slEvaluateFeature","slFreeResources","slGetFeatureFunction",
    "slGetFeatureRequirements","slGetFeatureVersion","slGetNativeInterface","slGetNewFrameToken",
    "slInit","slIsFeatureLoaded","slIsFeatureSupported","slSetConstants","slSetD3DDevice",
    "slSetFeatureLoaded","slSetTag","slSetTagForFrame","slSetVulkanInfo","slShutdown","slUpgradeInterface",
]
VK = ["vkAcquireNextImageKHR","vkAllocateCommandBuffers","vkAllocateDescriptorSets","vkAllocateMemory","vkBeginCommandBuffer","vkBindBufferMemory","vkBindBufferMemory2","vkBindImageMemory","vkBindImageMemory2","vkCmdBeginQuery","vkCmdBeginRenderPass","vkCmdBeginRenderPass2","vkCmdBeginRendering","vkCmdBindDescriptorSets","vkCmdBindIndexBuffer","vkCmdBindPipeline","vkCmdBindVertexBuffers","vkCmdBindVertexBuffers2","vkCmdBlitImage","vkCmdBlitImage2","vkCmdClearAttachments","vkCmdClearColorImage","vkCmdClearDepthStencilImage","vkCmdCopyBuffer","vkCmdCopyBuffer2","vkCmdCopyBufferToImage","vkCmdCopyBufferToImage2","vkCmdCopyImage","vkCmdCopyImage2","vkCmdCopyImageToBuffer","vkCmdCopyImageToBuffer2","vkCmdCopyQueryPoolResults","vkCmdDispatch","vkCmdDispatchBase","vkCmdDispatchIndirect","vkCmdDraw","vkCmdDrawIndexed","vkCmdDrawIndexedIndirect","vkCmdDrawIndexedIndirectCount","vkCmdDrawIndirect","vkCmdDrawIndirectCount","vkCmdEndQuery","vkCmdEndRenderPass","vkCmdEndRenderPass2","vkCmdEndRendering","vkCmdExecuteCommands","vkCmdFillBuffer","vkCmdNextSubpass","vkCmdNextSubpass2","vkCmdPipelineBarrier","vkCmdPipelineBarrier2","vkCmdPushConstants","vkCmdResetEvent","vkCmdResetEvent2","vkCmdResetQueryPool","vkCmdResolveImage","vkCmdResolveImage2","vkCmdSetBlendConstants","vkCmdSetCullMode","vkCmdSetDepthBias","vkCmdSetDepthBiasEnable","vkCmdSetDepthBounds","vkCmdSetDepthBoundsTestEnable","vkCmdSetDepthCompareOp","vkCmdSetDepthTestEnable","vkCmdSetDepthWriteEnable","vkCmdSetDeviceMask","vkCmdSetEvent","vkCmdSetEvent2","vkCmdSetFrontFace","vkCmdSetLineWidth","vkCmdSetPrimitiveRestartEnable","vkCmdSetPrimitiveTopology","vkCmdSetRasterizerDiscardEnable","vkCmdSetScissor","vkCmdSetScissorWithCount","vkCmdSetStencilCompareMask","vkCmdSetStencilOp","vkCmdSetStencilReference","vkCmdSetStencilTestEnable","vkCmdSetStencilWriteMask","vkCmdSetViewport","vkCmdSetViewportWithCount","vkCmdUpdateBuffer","vkCmdWaitEvents","vkCmdWaitEvents2","vkCmdWriteTimestamp","vkCmdWriteTimestamp2","vkCreateBuffer","vkCreateBufferView","vkCreateCommandPool","vkCreateComputePipelines","vkCreateDescriptorPool","vkCreateDescriptorSetLayout","vkCreateDescriptorUpdateTemplate","vkCreateDevice","vkCreateEvent","vkCreateFence","vkCreateFramebuffer","vkCreateGraphicsPipelines","vkCreateImage","vkCreateImageView","vkCreateInstance","vkCreatePipelineCache","vkCreatePipelineLayout","vkCreatePrivateDataSlot","vkCreateQueryPool","vkCreateRenderPass","vkCreateRenderPass2","vkCreateSampler","vkCreateSamplerYcbcrConversion","vkCreateSemaphore","vkCreateShaderModule","vkCreateSwapchainKHR","vkCreateWin32SurfaceKHR","vkDestroyBuffer","vkDestroyBufferView","vkDestroyCommandPool","vkDestroyDescriptorPool","vkDestroyDescriptorSetLayout","vkDestroyDescriptorUpdateTemplate","vkDestroyDevice","vkDestroyEvent","vkDestroyFence","vkDestroyFramebuffer","vkDestroyImage","vkDestroyImageView","vkDestroyInstance","vkDestroyPipeline","vkDestroyPipelineCache","vkDestroyPipelineLayout","vkDestroyPrivateDataSlot","vkDestroyQueryPool","vkDestroyRenderPass","vkDestroySampler","vkDestroySamplerYcbcrConversion","vkDestroySemaphore","vkDestroyShaderModule","vkDestroySurfaceKHR","vkDestroySwapchainKHR","vkDeviceWaitIdle","vkEndCommandBuffer","vkEnumerateDeviceExtensionProperties","vkEnumerateDeviceLayerProperties","vkEnumerateInstanceExtensionProperties","vkEnumerateInstanceLayerProperties","vkEnumerateInstanceVersion","vkEnumeratePhysicalDeviceGroups","vkEnumeratePhysicalDevices","vkFlushMappedMemoryRanges","vkFreeCommandBuffers","vkFreeDescriptorSets","vkFreeMemory","vkGetBufferDeviceAddress","vkGetBufferMemoryRequirements","vkGetBufferMemoryRequirements2","vkGetBufferOpaqueCaptureAddress","vkGetDescriptorSetLayoutSupport","vkGetDeviceBufferMemoryRequirements","vkGetDeviceGroupPeerMemoryFeatures","vkGetDeviceImageMemoryRequirements","vkGetDeviceImageSparseMemoryRequirements","vkGetDeviceMemoryCommitment","vkGetDeviceMemoryOpaqueCaptureAddress","vkGetDeviceProcAddr","vkGetDeviceQueue","vkGetDeviceQueue2","vkGetEventStatus","vkGetFenceStatus","vkGetImageMemoryRequirements","vkGetImageMemoryRequirements2","vkGetImageMemoryRequirements2KHR","vkGetImageSparseMemoryRequirements","vkGetImageSparseMemoryRequirements2","vkGetImageSubresourceLayout","vkGetInstanceProcAddr","vkGetPhysicalDeviceExternalBufferProperties","vkGetPhysicalDeviceExternalFenceProperties","vkGetPhysicalDeviceExternalSemaphoreProperties","vkGetPhysicalDeviceFeatures","vkGetPhysicalDeviceFeatures2","vkGetPhysicalDeviceFeatures2KHR","vkGetPhysicalDeviceFormatProperties","vkGetPhysicalDeviceFormatProperties2","vkGetPhysicalDeviceImageFormatProperties","vkGetPhysicalDeviceImageFormatProperties2","vkGetPhysicalDeviceMemoryProperties","vkGetPhysicalDeviceMemoryProperties2","vkGetPhysicalDeviceProperties","vkGetPhysicalDeviceProperties2","vkGetPhysicalDeviceProperties2KHR","vkGetPhysicalDeviceQueueFamilyProperties","vkGetPhysicalDeviceQueueFamilyProperties2","vkGetPhysicalDeviceSparseImageFormatProperties","vkGetPhysicalDeviceSparseImageFormatProperties2","vkGetPhysicalDeviceSurfaceCapabilitiesKHR","vkGetPhysicalDeviceSurfaceFormatsKHR","vkGetPhysicalDeviceSurfacePresentModesKHR","vkGetPhysicalDeviceSurfaceSupportKHR","vkGetPhysicalDeviceToolProperties","vkGetPipelineCacheData","vkGetPrivateData","vkGetQueryPoolResults","vkGetRenderAreaGranularity","vkGetSemaphoreCounterValue","vkGetSwapchainImagesKHR","vkInvalidateMappedMemoryRanges","vkMapMemory","vkMergePipelineCaches","vkQueueBindSparse","vkQueuePresentKHR","vkQueueSubmit","vkQueueSubmit2","vkQueueWaitIdle","vkResetCommandBuffer","vkResetCommandPool","vkResetDescriptorPool","vkResetEvent","vkResetFences","vkResetQueryPool","vkSetEvent","vkSetPrivateData","vkSignalSemaphore","vkTrimCommandPool","vkUnmapMemory","vkUpdateDescriptorSetWithTemplate","vkUpdateDescriptorSets","vkWaitForFences","vkWaitSemaphores"]

OUT = os.path.dirname(os.path.abspath(__file__)) + "/src"
os.makedirs(OUT, exist_ok=True)

lines = ["LIBRARY sl.interposer.dll", "EXPORTS"]
# D3D/DXGI -> our stubs
for n in D3D:
    lines.append(f"    {n}=w_{n}")
# sl* + vk* -> loader forwarders to the renamed real interposer.
# NOTE: the forwarder target module name must NOT contain dots, or the MSVC
# linker mis-parses "sl.interposer.orig.Fn" (it splits on a dot and loses the
# "sl." prefix -> LNK2001). So the real interposer is renamed to the dot-free
# "slinterposerorig.dll" and we forward as "slinterposerorig.Fn".
for n in SL + VK:
    lines.append(f"    {n}=slinterposerorig.{n}")

open(OUT + "/exports.def", "w", newline="\n").write("\n".join(lines) + "\n")
print(f"wrote src/exports.def: {len(D3D)} stubbed + {len(SL)+len(VK)} forwarded = {len(D3D)+len(SL)+len(VK)} exports")
