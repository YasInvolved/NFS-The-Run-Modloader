#include "Loader.h"
#include <NFSModloader/pch.h>

FILE* Loader::fOut;
FILE* Loader::fErr;

std::unique_ptr<Loader> Loader::s_instance = nullptr;

#pragma pack(push,8)
struct tagTHREADNAME_INFO
{
   DWORD dwType;
   LPCSTR szName;
   DWORD dwThreadID;
   DWORD dwFlags;
};
#pragma pack(pop)

static constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;

static LONG WINAPI VectoredHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
   if (pExceptionInfo->ExceptionRecord->ExceptionCode == MS_VC_EXCEPTION)
   {
      tagTHREADNAME_INFO* info = (tagTHREADNAME_INFO*)pExceptionInfo->ExceptionRecord->ExceptionInformation;
      if (info->dwType == 0x1000) 
      {
         DWORD threadId = info->dwThreadID == (DWORD)-1 ? GetCurrentThreadId() : info->dwThreadID;
         fmt::println("Thread ({}) has been renamed to {}", threadId, info->szName);
      }

      return EXCEPTION_CONTINUE_EXECUTION;

   }

   return EXCEPTION_CONTINUE_SEARCH;
}

Loader::Loader()
   : m_threadPool(std::make_unique<nfsloader::ThreadPool>(4)), 
     m_dllObserver(std::make_unique<nfsloader::DllObserver>())

{
   InitializeDebugTerminal();
   AddVectoredExceptionHandler(1, VectoredHandler);
}

Loader::~Loader()
{
   DestroyDebugTerminal();
}

const Loader& Loader::GetInstance()
{
   if (s_instance == nullptr)
      s_instance.reset(new Loader());

   return *s_instance;
}

nfsloader::ThreadPool& Loader::getThreadPool() const
{
   return *m_threadPool;
}

nfsloader::DllObserver& Loader::getDllObserver() const
{
   return *m_dllObserver;
}

// static functions
void Loader::InitializeDebugTerminal()
{
#ifdef NFS_MODLOADER_DEBUG
   AllocConsole();
   freopen_s(&fOut, "CONOUT$", "w", stdout);
   freopen_s(&fErr, "CONOUT$", "w", stderr);
#endif
}

void Loader::DestroyDebugTerminal()
{
#ifdef NFS_MODLOADER_DEBUG
   fclose(fErr);
   fclose(fOut);
   FreeConsole();
#endif
}