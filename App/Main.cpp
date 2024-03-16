#include <Xeph2D.h>
#include "gen/ComponentManifest.generated.h"

#ifdef _CONSOLE
int wmain(int argc, wchar_t* argv[])
{
	Xeph2D::Runtime::ParseArgs(argc, argv);
#else
#include <windows.h>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Xeph2D::Runtime::ParseArgs(pCmdLine);
#endif //_CONSOLE

	Xeph2D::Runtime::Initialize(__X2D_POPULATE_COMP_PTR);
	Xeph2D::Runtime::Update();
	Xeph2D::Runtime::Terminate();

	return 0;
}