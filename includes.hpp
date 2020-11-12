#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#pragma warning( disable : 4244 )
#pragma warning( disable : 4996 )
#include <iostream>
#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")	
#include <Psapi.h>
#include <tchar.h>
#include <math.h>
#include <sstream>
#include <string>
#include <intrin.h>
#include <chrono>
#include <stdlib.h> 
#include <bitset>
#include <algorithm>
#include <windef.h>

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);