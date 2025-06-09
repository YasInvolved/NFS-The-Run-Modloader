#ifndef _NFS_MODLOADER_LOADER_H_INCLUDED_
#define _NFS_MODLOADER_LOADER_H_INCLUDED_

#include "ThreadPool.h"

class Loader
{
private:
   static FILE* fOut;
   static FILE* fErr;

   friend struct std::default_delete<Loader>;
   static std::unique_ptr<Loader> s_instance;

   std::unique_ptr<nfsloader::ThreadPool> m_threadPool;
   std::unique_ptr<nfsloader::DllObserver> m_dllObserver;
public:
   static const Loader& GetInstance();

   nfsloader::ThreadPool& getThreadPool() const;
   nfsloader::DllObserver& getDllObserver() const;
private:
   Loader();
   ~Loader();

   static void InitializeDebugTerminal();
   static void DestroyDebugTerminal();
};

#endif