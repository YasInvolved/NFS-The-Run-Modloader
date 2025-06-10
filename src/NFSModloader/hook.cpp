#include <NFSModloader/pch.h>

using PFN_CreateDXGIFactory = HRESULT(WINAPI*)(REFIID riid, void** ppFactory);
using PFN_CreateDXGIFactory1 = PFN_CreateDXGIFactory;
using PFN_CreateDXGIFactory2 = HRESULT(WINAPI*)(UINT Flags, REFIID riid, void** ppFactory);

using PFN_DXGIGetDebugInterface = decltype(&DXGIGetDebugInterface);

namespace real
{
   static HMODULE s_dxgi = nullptr;
   static HMODULE s_dxgiDebug = nullptr;
   static PFN_CreateDXGIFactory CreateDXGIFactory = nullptr;
   static PFN_CreateDXGIFactory1 CreateDXGIFactory1 = nullptr;
   static PFN_CreateDXGIFactory2 CreateDXGIFactory2 = nullptr;
   static PFN_DXGIGetDebugInterface DXGIGetDebugInterface = nullptr;
}

static ComPtr<IDXGIDebug> s_dxgiDebugIntf;

static void loadDxgi()
{
   if (real::s_dxgi == nullptr)
      real::s_dxgi = nfsloader::utils::LoadSystemDll("dxgi.dll");

#ifdef NFS_MODLOADER_DEBUG
   if (real::s_dxgiDebug == nullptr)
      real::s_dxgiDebug = nfsloader::utils::LoadSystemDll("dxgidebug.dll");

   if (real::DXGIGetDebugInterface == nullptr)
      real::DXGIGetDebugInterface = nfsloader::utils::GetRequiredDllFunctionPointer<PFN_DXGIGetDebugInterface>(real::s_dxgiDebug, "DXGIGetDebugInterface");
#endif

   if (real::CreateDXGIFactory == nullptr)
      real::CreateDXGIFactory = nfsloader::utils::GetRequiredDllFunctionPointer<PFN_CreateDXGIFactory>(real::s_dxgi, "CreateDXGIFactory");

   if (real::CreateDXGIFactory1 == nullptr)
      real::CreateDXGIFactory1 = nfsloader::utils::GetRequiredDllFunctionPointer<PFN_CreateDXGIFactory1>(real::s_dxgi, "CreateDXGIFactory1");

   if (real::CreateDXGIFactory2 == nullptr)
      real::CreateDXGIFactory2 = nfsloader::utils::GetRequiredDllFunctionPointer<PFN_CreateDXGIFactory2>(real::s_dxgi, "CreateDXGIFactory2");
}

using DllCallbackReason = nfsloader::DllCallbackReason;
static void debugDllCallback(const std::string& name, DllCallbackReason reason)
{
   fmt::println("{} has been {}", name, reason == DllCallbackReason::LOADED ? "loaded" : "unloaded");
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory(REFIID riid, void** ppFactory)
{
   if (real::s_dxgi == nullptr || real::CreateDXGIFactory == nullptr)
      loadDxgi();

   return real::CreateDXGIFactory(riid, ppFactory);
}

// Game calls CreateDXGIFactory1. Other functions are implemented for compatibility because the game requires them for some reason
extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory1(REFIID riid, void** ppFactory)
{
   if (real::s_dxgi == nullptr || real::CreateDXGIFactory1 == nullptr)
      loadDxgi();

   auto& loader = Loader::GetInstance();
   loader.getDllObserver().addCallback(debugDllCallback);

#ifdef NFS_MODLOADER_DEBUG
   real::DXGIGetDebugInterface(IID_PPV_ARGS(&s_dxgiDebugIntf));
#endif

   return real::CreateDXGIFactory1(riid, ppFactory);
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory2(UINT Flags, REFIID riid, void** ppFactory)
{
   if (real::s_dxgi == nullptr || real::CreateDXGIFactory2 == nullptr)
      loadDxgi();

   return real::CreateDXGIFactory2(Flags, riid, ppFactory);
}

// link to the correct function
#pragma comment(linker, "/EXPORT:CreateDXGIFactory=_CreateDXGIFactory@8")
#pragma comment(linker, "/EXPORT:CreateDXGIFactory1=_CreateDXGIFactory1@8")
#pragma comment(linker, "/EXPORT:CreateDXGIFactory2=_CreateDXGIFactory2@12")