#include <Xeph2D.h>
#include "gen/ScriptManifest.generated.h"

#ifdef _CONSOLE
int wmain(int argc, wchar_t argv[])
#else
#include <windows.h>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif //_CONSOLE
{
	Xeph2D::Runtime::Initialize(__X2D_REGISTER_COMP_NAMES, __X2D_POPULATE_COMP_PTR);
	return 0;
}