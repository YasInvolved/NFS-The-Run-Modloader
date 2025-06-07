#include <NFSModloader/dllLoadMonitor.h>

using DllCallback = nfsloader::DllCallback;
using DllObserver = nfsloader::DllObserver;
using DllCallbackReason = nfsloader::DllCallbackReason;

HMODULE DllObserver::s_ntdll = nullptr;
LdrRegisterDllNotification_t DllObserver::s_registerDllNotification = nullptr;
LdrUnregisterDllNotification_t DllObserver::s_unregisterDllNotification = nullptr;
bool DllObserver::s_windowsCbRegistered = false;

using DllObserver = nfsloader::DllObserver;

DllObserver::DllObserver()
{
   if (s_ntdll == nullptr)
      s_ntdll = utils::GetRequiredModuleHandle("ntdll.dll");

   if (s_registerDllNotification == nullptr)
      s_registerDllNotification = utils::GetRequiredDllFunctionPointer<LdrRegisterDllNotification_t>(s_ntdll, "LdrRegisterDllNotification");

   if (s_unregisterDllNotification == nullptr)
      s_unregisterDllNotification = utils::GetRequiredDllFunctionPointer<LdrUnregisterDllNotification_t>(s_ntdll, "LdrUnregisterDllNotification");

   s_registerDllNotification(0, DllObserver::WindowsApiCallback, this, &m_cookie);
}

DllObserver::~DllObserver()
{
   s_unregisterDllNotification(&m_cookie);
}

void DllObserver::addCallback(DllCallback c)
{
   std::lock_guard<std::mutex> lock(m_callbacksAccess);
   m_callbacks.emplace_back(c);
}

void NTAPI DllObserver::WindowsApiCallback(ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData, PVOID Context)
{
   DllObserver* observer = reinterpret_cast<DllObserver*>(Context);
   DllCallbackReason reason = NotificationReason == 1 ? DllCallbackReason::LOADED : DllCallbackReason::UNLOADED;
   
   // NotificationData.Loaded and Unloaded are the same
   UNICODE_STRING& rawNameStr = *NotificationData->Loaded.BaseDllName;
   std::string dllName = utils::UnicodeToMultibyte(rawNameStr.Buffer, static_cast<size_t>(rawNameStr.Length / sizeof(WCHAR)));
   
   for (const auto& callback : observer->m_callbacks)
   {
      callback(dllName, reason);
   }
}