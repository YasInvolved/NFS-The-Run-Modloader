# Research

> [!IMPORTANT]
> My reaserch addresses 1.1.0.0 executable

## Renderer architecture
- `0xF427DA14` is an address of IDXGIFactory* in game's memory. It's static so you should not worry about offsets.
- `0xF427DA18` is also an address of something, probably IDXGIAdapter*

### Graphics adapter info (0593F8C8) (?)
```cpp
struct AdapterInfo
{
  uint32_t unkn1;
  uint32_t unkn2;
  uint32_t unkn3;
  uint32_t unkn4;
  wchar_t* adapterName;
}
```

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
