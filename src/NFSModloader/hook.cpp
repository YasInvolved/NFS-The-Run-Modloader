#include <NFSModloader/pch.h>

using DirectInput8Create_t = HRESULT(WINAPI*)(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

static HMODULE s_dinput8Module = nullptr;

namespace real
{
   static DirectInput8Create_t DirectInput8Create = nullptr;
}

static void loadDinput8()
{
   if (s_dinput8Module == nullptr)
      s_dinput8Module = nfsloader::utils::LoadSystemDll("dinput8.dll");

   if (real::DirectInput8Create == nullptr)
      real::DirectInput8Create = nfsloader::utils::GetRequiredDllFunctionPointer<DirectInput8Create_t>(s_dinput8Module, "DirectInput8Create");
}

// I discovered that this function might be called twice by 2 different threads,
// so thread safe mechanism is needed here
static std::atomic<bool> s_initialized = false;

using DllCallbackReason = nfsloader::DllCallbackReason;

static void dllCallback(const std::string& dllName, DllCallbackReason reason)
{
   fmt::println("{} has been {}", dllName, reason == DllCallbackReason::LOADED ? "loaded" : "unloaded");
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DirectInput8Create(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
   if (s_dinput8Module == nullptr || real::DirectInput8Create == nullptr)
      loadDinput8();

   // small test
   if (not s_initialized.load(std::memory_order_acquire))
   {
      s_initialized.store(true, std::memory_order_relaxed);
      HMODULE thisHandle = nfsloader::utils::GetThisDllHandle();
      DisableThreadLibraryCalls(thisHandle);
      const auto& loader = Loader::GetInstance();
      loader.getDllObserver().addCallback(dllCallback);
   }

   return real::DirectInput8Create(hInst, dwVersion, riidltf, ppvOut, punkOuter);
}

// link to the correct function
#pragma comment(linker, "/EXPORT:DirectInput8Create=_DirectInput8Create@20")