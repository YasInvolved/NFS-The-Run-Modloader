#ifndef _NFS_MODLOADER_DLL_LOAD_MONITOR_H_INCLUDED_
#define _NFS_MODLOADER_DLL_LOAD_MONITOR_H_INCLUDED_

// definitions of undocumented winapi structures and functions

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

using LdrRegisterDllNotification_t = NTSTATUS(NTAPI*)(ULONG Flags, LDR_DLL_NOTIFICATION_FUNCTION NotificationFunction, PVOID Context, PVOID* Cookie);
using LdrUnregisterDllNotification_t = NTSTATUS(NTAPI*)(PVOID Cookie);

namespace nfsloader
{
   enum class DllCallbackReason { LOADED, UNLOADED };

   using DllCallback = std::function<void(const std::string&, DllCallbackReason)>;

   class DllObserver
   {
   private:
      static bool s_windowsCbRegistered;
      static HMODULE s_ntdll;
      
      std::mutex m_callbacksAccess;
      std::vector<DllCallback> m_callbacks;
      PVOID m_cookie;
   public:
      DllObserver();
      ~DllObserver();
      
      void addCallback(DllCallback c);

   private:
      static LdrRegisterDllNotification_t s_registerDllNotification;
      static LdrUnregisterDllNotification_t s_unregisterDllNotification;
      static void NTAPI WindowsApiCallback(ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData, PVOID Context);
   };
}

#endif