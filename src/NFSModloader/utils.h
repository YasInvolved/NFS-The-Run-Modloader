#ifndef _NFS_MODLOADER_UTILS_H_INCLUDED_
#define _NFS_MODLOADER_UTILS_H_INCLUDED_

#include "framework.h"
#include <string>
#include <filesystem>
#include <cassert>

namespace nfsloader::utils
{
   namespace fs = std::filesystem;

   inline void errorMsgBox(const std::string_view text)
   {
      MessageBoxA(nullptr, text.data(), "Fatal Error", MB_ICONEXCLAMATION | MB_OK);
   }

   inline fs::path getSystemDirectory()
   {
      char buf[MAX_PATH];
      GetSystemDirectoryA(buf, MAX_PATH);

      return fs::path(std::string(buf));
   }

   inline fs::path getSystemModuleAbsolutePath(const std::string_view dll)
   {
      fs::path path = getSystemDirectory() / dll;
      assert(fs::exists(path));

      return path;
   }

   [[nodiscard]] inline std::string UnicodeToMultibyte(const wchar_t* wstr, size_t length)
   {
      int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(length), nullptr, 0, nullptr, nullptr);
      std::string buf(size_needed, 0);
      WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(length), buf.data(), size_needed, nullptr, nullptr);

      return buf;
   }

   [[nodiscard]] inline HMODULE GetThisDllHandle()
   {
      HMODULE hModule = nullptr;
      GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCTSTR>(&GetThisDllHandle), &hModule);

      return hModule;
   }

   [[nodiscard]] inline HMODULE LoadSystemDll(const std::string_view dllName)
   {
      HMODULE hModule = LoadLibraryA(getSystemModuleAbsolutePath(dllName.data()).string().c_str());
      if (hModule == nullptr)
      {
         errorMsgBox(fmt::format("Failed to load {}", dllName));
         std::abort();
      }

      return hModule;
   }

   [[nodiscard]] inline HMODULE GetRequiredModuleHandle(const std::string_view dllName)
   {
      HMODULE module = GetModuleHandleA(dllName.data());
      if (!module)
      {
         errorMsgBox(fmt::format("Failed to get module handle for {}", dllName));
         std::abort();
      }

      return module;
   }

   template <typename FuncType>
   [[nodiscard]] FuncType GetRequiredDllFunctionPointer(HMODULE dllHandle, const std::string_view funcName)
   {
      FuncType f = reinterpret_cast<FuncType>(GetProcAddress(dllHandle, funcName.data()));
      if (f == NULL)
      {
         errorMsgBox(fmt::format("Failed to get function pointer for {}", funcName.data()));
         std::abort();
      }

      return f;
   }
}

#endif