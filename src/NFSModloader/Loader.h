#ifndef _NFS_MODLOADER_LOADER_H_INCLUDED_
#define _NFS_MODLOADER_LOADER_H_INCLUDED_

class Loader
{
private:
   static FILE* fOut;
   static FILE* fErr;

public:
   static void Initialize();
   static void Destroy();

private:
   static void InitializeTerminal();
   static void DestroyTerminal();
};

#endif