# Fake sl.interposer.dll — 诊断/重定向代理

目的：yysls.exe 用 NVIDIA Streamline，其 `sl.interposer.dll` 重导出了标准
D3D12/DXGI 入口函数，游戏链接的是这些。Streamline 内部用"手动遍历导出表(EAT)"
调真实 D3D12/DXGI，绕过 RenderDoc 的 IAT / GetProcAddress hook（见 RenderDoc
issue #3257）。本代理 DLL 顶替 sl.interposer.dll，接管这些入口，用**标准
GetProcAddress** 转发到真实 d3d12core/dxgi —— 这样 RenderDoc 的现有 hook 就能
生效，device/swapchain 被 wrap，F12 截帧可用。

## 工作原理

- **14 个 D3D/DXGI 导出** (`D3D12CreateDevice`/`D3D12GetInterface`/`CreateDXGIFactory*` 等)
  → 本 DLL 的 C 桩函数 (`w_*`)：打印 `[FAKESL]` 日志 + 用 GetProcAddress 转发到
  **真实系统 d3d11/d3d12/dxgi.dll**（不是真实 interposer）。
- **19 个 sl\* + 229 个 vk\* 导出** → `.def` 里的 loader forwarder，直接转发到
  改名后的真实 interposer `slinterposerorig.dll`（无点号模块名 —— MSVC 链接器会
  错误解析带点号的 forwarder 目标；零代码）。
- `DllMain` 里加载真实系统 DLL + `senderdod.dll`（把 RenderDoc 拉进进程）。

代价：诊断构建下，D3D12 设备创建绕过了真实 Streamline，DLSS/帧生成可能失效。
对截帧调试无影响（游戏用原生 D3D12 渲染即可）。

## 构建

用 VS2022 (v142 工具集)：
```
MSBuild sl_interposer_wrapper.vcxproj /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v142
```
产物：`x64\Release\sl.interposer.dll`

## 安装（在游戏目录 Streamline\ 下）

1. 备份并改名真实 interposer：
   `sl.interposer.dll` → `slinterposerorig.dll`
   （无点号模块名——MSVC 链接器要求；游戏目录已有 `sl.interposer.dll.bak` /
   `sl.interposer - 副本.dll` 备份）
2. 把构建出的本 DLL 复制进去，命名为 `sl.interposer.dll`
3. 确保 `senderdod.dll` 能被本 DLL 的 LoadLibrary 找到（放游戏 exe 目录，或
   已在 RenderDoc 注入路径上）

目标目录：`D:\Games\yysls\yysls_medium\Engine\Binaries\Win64r\Streamline\`

## 验证

1. 用 qsenderdod.exe 启动游戏
2. 看游戏目录下 `Streamline\fakesl.log` —— 确认本 DLL 被加载、哪些 D3D/DXGI
   导出被调用（`[FAKESL #n] <fn> -> forwarding to <dll>!<fn>`）
3. 进游戏按 F12
4. 看 RenderDoc 日志（`%TEMP%\RenderDoc\...`）过滤 `[ksh]`：
   - 期待现在能看到 `D3D12CreateDevice`/`D3D12GetInterface:core` 被 hook、
     `WrappedID3D12Device::ctor`（capture 侧）、swapchain wrap、F12 截帧

## 判读

- **fakesl.log 里有 D3D12CreateDevice/CreateDXGIFactory 调用** → 游戏确实通过
  interposer 导出走，我们接管成功。若此时 RenderDoc 仍没 hook 到，说明转发到
  真实 d3d12core 的调用又被壳的 EAT 遍历绕过（可能性低，因为我们用标准
  GetProcAddress）。
- **fakesl.log 只有 DllMain 加载日志、无 D3D 调用** → 游戏没通过 interposer 的
  这些导出建设备（可能用了 vk 路径，或壳对 interposer 也做 EAT 遍历）。需要看
  fakesl.log 里 vk\* 是否活跃，或换思路。
