#include <NFSModloader/dllLoadMonitor.h>

struct LDR_DLL_LOADED_NOTIFICATION_DATA
{
   ULONG Flags;
   PUNICODE_STRING FullDllName;
   PUNICODE_STRING BaseDllName;
   PVOID DllBase;
   ULONG SizeOfImage;
};

struct LDR_DLL_UNLOADED_NOTIFICATION_DATA
{
   ULONG Flags;
   PUNICODE_STRING FullDllName;
   PUNICODE_STRING BaseDllName;
   PVOID DllBase;
   ULONG SizeOfImage;
};

union LDR_DLL_NOTIFICATION_DATA
{
   LDR_DLL_LOADED_NOTIFICATION_DATA Loaded;
   LDR_DLL_UNLOADED_NOTIFICATION_DATA Unloaded;
};

using PCLDR_DLL_NOTIFICATION_DATA = const LDR_DLL_NOTIFICATION_DATA*;
using LDR_DLL_NOTIFICATION_FUNCTION = VOID(NTAPI*)(ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData, PVOID Context);


// registerers
using LdrRegisterDllNotification_t = NTSTATUS(NTAPI*)(ULONG Flags, LDR_DLL_NOTIFICATION_FUNCTION NotificationFunction, PVOID Context, PVOID* Cookie);
using LdrUnregisterDllNotification_t = NTSTATUS(NTAPI*)(PVOID Cookie);

static HMODULE s_ntdll = nullptr;
static LdrRegisterDllNotification_t LdrRegisterDllNotification = nullptr;
static LdrUnregisterDllNotification_t LdrUnregisterDllNotification = nullptr;

static PVOID s_cookie = nullptr;

static inline std::string unicodeToMultibyte(const UNICODE_STRING& wstr)
{
   int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.Buffer, (int)(wstr.Length / sizeof(WCHAR)), nullptr, 0, nullptr, nullptr);
   std::string buf(size_needed, 0);
   WideCharToMultiByte(CP_UTF8, 0, wstr.Buffer, (int)(wstr.Length / sizeof(WCHAR)), buf.data(), size_needed, nullptr, nullptr);

   return buf;
}

static void NTAPI dllLoadCallback(ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData, PVOID Context)
{
   if (NotificationReason == 1)
   {
      fmt::println("Loaded {}", unicodeToMultibyte(*NotificationData->Loaded.BaseDllName));
   }
}

void nfsloader::initializeDllNotifications()
{
   if (s_ntdll == nullptr)
      s_ntdll = nfsloader::utils::GetRequiredModuleHandle("ntdll.dll");

   if (LdrRegisterDllNotification == nullptr)
      LdrRegisterDllNotification = nfsloader::utils::GetRequiredDllFunctionPointer<LdrRegisterDllNotification_t>(s_ntdll, "LdrRegisterDllNotification");

   if (LdrUnregisterDllNotification == nullptr)
      LdrUnregisterDllNotification = nfsloader::utils::GetRequiredDllFunctionPointer<LdrUnregisterDllNotification_t>(s_ntdll, "LdrUnregisterDllNotification");

   LdrRegisterDllNotification(0, dllLoadCallback, nullptr, &s_cookie);
}