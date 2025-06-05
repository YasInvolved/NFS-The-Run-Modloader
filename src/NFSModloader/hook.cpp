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
      }
   }
}

// NOTE: Don't add any calling conventions like __stdcall or WINAPI, NFS looks for plain DirectInput8Create
extern "C" __declspec(dllexport)
HRESULT DirectInput8Create(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
   if (s_dinput8Module == nullptr || real::DirectInput8Create == nullptr)
      loadDinput8();

   return real::DirectInput8Create(hInst, dwVersion, riidltf, ppvOut, punkOuter);
}