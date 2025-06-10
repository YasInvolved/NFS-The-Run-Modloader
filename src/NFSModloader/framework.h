#ifndef _NFS_MODLOADER_FRAMEWORK_H_INCLUDED_
#define _NFS_MODLOADER_FRAMEWORK_H_INCLUDED_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Unknwn.h>
#include <winternl.h>

// COM
#include <dxgidebug.h>
#include <wrl/client.h>

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#endif