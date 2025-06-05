#include "Loader.h"

FILE* Loader::fOut;
FILE* Loader::fErr;

void Loader::Initialize()
{
   InitializeTerminal();
}

void Loader::Destroy()
{
   DestroyTerminal();
}

void Loader::InitializeTerminal()
{
   AllocConsole();
   freopen_s(&fOut, "CONOUT$", "w", stdout);
   freopen_s(&fErr, "CONOUT$", "w", stderr);
}

void Loader::DestroyTerminal()
{
   fclose(fErr);
   fclose(fOut);
   FreeConsole();
}