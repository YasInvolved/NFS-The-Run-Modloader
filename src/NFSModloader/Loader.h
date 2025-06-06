#ifndef _NFS_MODLOADER_LOADER_H_INCLUDED_
#define _NFS_MODLOADER_LOADER_H_INCLUDED_

class Loader
{
private:
   static FILE* fOut;
   static FILE* fErr;

public:
   static Loader& GetInstance();
private:
   friend struct std::default_delete<Loader>;

   Loader();
   ~Loader();

   static void InitializeDebugTerminal();
   static void DestroyDebugTerminal();

   static std::unique_ptr<Loader> s_instance;
};

#endif