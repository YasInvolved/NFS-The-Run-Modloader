#include "pch.h"

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
   if (fdwReason == DLL_PROCESS_ATTACH)
   {
      // thread calls are not needed for now
      DisableThreadLibraryCalls(hInstance);
   }

   return TRUE;
}