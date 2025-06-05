#ifndef _NFS_MODLOADER_UTILS_H_INCLUDED_
#define _NFS_MODLOADER_UTILS_H_INCLUDED_

#include <string>
#include <filesystem>
#include <cassert>

namespace nfsloader::utils
{
   namespace fs = std::filesystem;

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
}

#endif