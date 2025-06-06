#include "pch.h"

using DirectInput8Create_t = HRESULT(WINAPI*)(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

static HMODULE s_dinput8Module = nullptr;

namespace real
{
   static DirectInput8Create_t DirectInput8Create = nullptr;
}

static inline void errorMsgBox(const std::string_view text)
{
   MessageBoxA(nullptr, text.data(), "Fatal Error", MB_ICONEXCLAMATION | MB_OK);
}

static void loadDinput8()
{
   if (s_dinput8Module == nullptr)
   {
      auto modPath = nfsloader::utils::getSystemModuleAbsolutePath("dinput8.dll");
      s_dinput8Module = LoadLibraryA(modPath.string().c_str());
      if (s_dinput8Module == 0)
      {
         errorMsgBox(fmt::format("Failed to load {}", modPath.string()));
         std::abort();
      }
   }

   if (real::DirectInput8Create == nullptr && s_dinput8Module != nullptr)
   {
      real::DirectInput8Create = reinterpret_cast<DirectInput8Create_t>(GetProcAddress(s_dinput8Module, "DirectInput8Create"));
      if (real::DirectInput8Create == nullptr)
      {
         errorMsgBox("Failed to load DirectInput8Create");
         std::abort();
      }
   }
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DirectInput8Create(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
   if (s_dinput8Module == nullptr || real::DirectInput8Create == nullptr)
      loadDinput8();

   // conduct a small test
   Loader::GetInstance().getThreadPool().enqueue([]() { fmt::println("Hello from worker thread with id"); });

   return real::DirectInput8Create(hInst, dwVersion, riidltf, ppvOut, punkOuter);
}

// link to the correct function
#pragma comment(linker, "/EXPORT:DirectInput8Create=_DirectInput8Create@20")