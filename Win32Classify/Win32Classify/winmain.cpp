#include "winmain.h"
#include "MainWindow.h"

INT WINAPI
_tWinMain(HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPTSTR szCmdLine,
	INT nShowCmd) {

	MainWindow app;

	app.Initialize();

	return app.Run();
}