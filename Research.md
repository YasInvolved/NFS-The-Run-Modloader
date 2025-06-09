# Research

> [!IMPORTANT]
> My reaserch addresses 1.1.0.0 executable

## Renderer architecture
- `0x241F784` is an address of IDXGIFactory in game's memory. It's static so you should not worry about offsets.
- `0x241F730` is an address of string that is used in engine's log function that gives us a hint that address of IDXGIFactory is a class field. The string reads: `CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&m_dxgiFactory.assignGet()))`

> [!NOTE]
> Currently I'm probably on track of engine's IDXGIFactory wrapper that's a part of renderer architecture