#include "Loader.h"
#include "pch.h"

FILE* Loader::fOut;
FILE* Loader::fErr;

std::unique_ptr<Loader> Loader::s_instance = nullptr;

Loader::Loader()
{
   InitializeDebugTerminal();
}

Loader::~Loader()
{
   DestroyDebugTerminal();
}

Loader& Loader::GetInstance()
{
   if (s_instance == nullptr)
      s_instance.reset(new Loader());

   return *s_instance;
}

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