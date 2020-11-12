#include "includes.hpp"
#include "d3d9.h"
#include <windef.h>
int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);
const MARGINS Margin = { 0, 0, Width, Height };

RECT rc;
HWND hWnd;
char tWindowName[256] = "Dota 2";
HWND tWnd;
RECT tSize;
MSG Message;
LONG topR = 0, leftR = 0, rightR = 0, bottomR = 0;

void set_window_to_target()
{
	tWnd = FindWindow(0, tWindowName);
	if (tWnd)
	{
		GetWindowRect(tWnd, &tSize);
		Width = tSize.right - tSize.left + rightR;
		Height = tSize.bottom - tSize.top + bottomR;

		tSize.left -= leftR;
		tSize.top -= topR;

		DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
		if (dwStyle & WS_BORDER)
		{
			tSize.top += 23;
			Height -= 23;
		}
		MoveWindow(hWnd, tSize.left, tSize.top, Width, Height, true);
	}
	else { 
		exit(1);
	}

	while (true)
	{
		tWnd = FindWindow(0, tWindowName);
		if (tWnd)
		{
			GetWindowRect(tWnd, &tSize);
			Width = tSize.right - tSize.left + rightR;
			Height = tSize.bottom - tSize.top + bottomR;

			tSize.left -= leftR;
			tSize.top -= topR;

			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				tSize.top += 23;
				Height -= 23;
			}
			MoveWindow(hWnd, tSize.left, tSize.top, Width, Height, true);
		}
		else
		{
			exit(1);
		}
		Sleep(1);
	}
}

DWORD PID = 0;
DWORD engine_dll, engine_dll_size, vstdlib_dll, vstdlib_dll_size, client_dll, client_dll_size; string folder;
HANDLE get_process_handle()
{
	HANDLE h = 0; 
	DWORD pid = 0; 
	HWND hWnd = FindWindow(0, _T(tWindowName)); 
	if (hWnd == 0) {
		return h; 
	}
	GetWindowThreadProcessId(hWnd, &pid); 
	h = OpenProcess(PROCESS_ALL_ACCESS, 0, pid); 
	if (h == 0) {
		return h; 
	}

	PID = pid;
	HMODULE hMods[1024]; 
	int i; 
	if (EnumProcessModules(h, hMods, sizeof(hMods), &pid) == FALSE) {

	}

	else {
		for (i = 0; i < (pid / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH]; 
			if (GetModuleFileNameEx(h, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				if (_tcsstr(szModName, ("\\engine.dll")) != NULL) {
					MODULEINFO lpmodinfo = { 0 }; 
					GetModuleInformation(h, hMods[i], &lpmodinfo, sizeof(lpmodinfo)); 

					engine_dll = (DWORD)hMods[i]; 
					engine_dll_size = lpmodinfo.SizeOfImage; 
				}

				if (_tcsstr(szModName, ("\\vstdlib.dll")) != NULL)
				{
					MODULEINFO lpmodinfo = { 0 }; 
					GetModuleInformation(h, hMods[i], &lpmodinfo, sizeof(lpmodinfo)); 

					vstdlib_dll = (DWORD)hMods[i]; 
					vstdlib_dll_size = lpmodinfo.SizeOfImage; 
				}

				if (_tcsstr(szModName, ("\\client.dll")) != NULL)
				{
					MODULEINFO lpmodinfo = { 0 }; 
					GetModuleInformation(h, hMods[i], &lpmodinfo, sizeof(lpmodinfo)); 

					client_dll = (DWORD)hMods[i]; 
					client_dll_size = lpmodinfo.SizeOfImage; 

					folder = szModName; 
					folder = folder.substr(0, folder.length() - 14); 
					folder += "maps"; 
				}

			}
		}
	}
	return h; 
}

HANDLE hProcess;
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		//RENDER

		if (tWnd == GetForegroundWindow())
		{
			char* value = tWindowName; 

			HWND newhwnd = FindWindow(NULL, value); 
			if (newhwnd != NULL) {
				GetWindowRect(newhwnd, &rc); 
			}
			else {
				ExitProcess(0); 
			}

			p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0); 
			p_Device->BeginScene(); 

			DirectX::Render(); // draw

			p_Device->EndScene(); 
			p_Device->Present(NULL, NULL, NULL, NULL); 
		}
		else
		{
			p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0); 
			p_Device->BeginScene(); 

			p_Device->EndScene(); 
			p_Device->PresentEx(0, 0, 0, 0, 0); 
		}

		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin); 

		hProcess = get_process_handle(); 

		if (!hProcess) {
			exit(1); 
		}

		break;

	case WM_DESTROY:
		CloseHandle(hProcess); 
		PostQuitMessage(1); 
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam); 
		break;
	}
	return 0; 
}

struct Modules
{
	uintptr_t clientModule;
	uintptr_t serverModule;
} modules;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hSecInstance, LPSTR nCmdLine, INT nCmdShow);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hSecInstance, LPSTR nCmdLine, INT nCmdShow)
{
	topR = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 250.0f)); 
	leftR = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 250.0f)); 
	bottomR = topR * (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f)) + 1.0f); 
	rightR = leftR * (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f)) + 1.0f); 

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_window_to_target, 0, 0, 0); 

	WNDCLASSEX wClass; 
	wClass.cbClsExtra = NULL; 
	wClass.cbSize = sizeof(WNDCLASSEX); 
	wClass.cbWndExtra = NULL; 
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); 
	wClass.hCursor = LoadCursor(0, IDC_ARROW); 
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION); 
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION); 
	wClass.hInstance = hInstance; 
	wClass.lpfnWndProc = WinProc; 
	wClass.lpszClassName = "WINDOW1"; 
	wClass.lpszMenuName = "WINDOW1"; 
	wClass.style = CS_VREDRAW | CS_HREDRAW; 

	if (!RegisterClassEx(&wClass))
		exit(1); 

	tWnd = FindWindow(0, tWindowName); 
	if (tWnd)
	{
		GetWindowRect(tWnd, &tSize); 
		if (tSize.right - tSize.left < 600)
		{
			char ErrorMsg[125]; 
			sprintf(ErrorMsg, "Make sure %s is in windowed mode!", tWindowName); 
			MessageBox(0, ErrorMsg, "Error - Cannot find the game window!", MB_OK | MB_ICONERROR); 
			exit(1);
		}

		Width = tSize.right - tSize.left + rightR; 
		Height = tSize.bottom - tSize.top + bottomR; 

		tSize.left -= leftR; 
		tSize.top -= topR; 

		hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_LAYERED, "WINDOW1", "WINDOW1", WS_POPUP, 1, 1, Width, Height, 0, 0, 0, 0); 
		SetLayeredWindowAttributes(hWnd, 0, 1.0f, LWA_ALPHA); 
		SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY); 
		ShowWindow(hWnd, SW_SHOW); 
	}

	DirectX_init(hWnd); 

	for (;; )
	{
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message); 
			TranslateMessage(&Message); 
		}
		Sleep(1); 
	}

	return 0; 
}