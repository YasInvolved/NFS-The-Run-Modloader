# Research

> [!IMPORTANT]
> My reaserch addresses 1.1.0.0 executable

## Renderer architecture
- `0x241F784` is an address of IDXGIFactory** in game's memory. It's static so you should not worry about offsets.

> [!NOTE]
> Currently I'm probably on track of engine's IDXGIFactory wrapper that's a part of renderer architecture

## Debug Strings
- `0x241F730`: `"CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&m_dxgiFactory.assignGet()))"`
- `0x241F708`: `"adapterPtr->GetDesc(&adapterDesc)"`

## Misc
- `0x6EC2E0` is an address of some sort of engine's internal log or assert function that looks like this
```cpp
void Log(const char* fmt, uint32_t unknown /*might be a this pointer, I'm investigating it*/);
```