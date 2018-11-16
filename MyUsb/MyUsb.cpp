// MyUsb.cpp : Defines the entry point for the application.
//
/**
	Test di utilizzo della libreria libusb
	Sito: https://libusb.info/

*/

#include "stdafx.h"
#include "MyUsb.h"
#include <windowsx.h>

#include "include\libusb-1.0\libusb.h"

#pragma comment( lib, "MS32\\dll\\libusb-1.0.lib")

#define MAX_LOADSTRING 100

#define VID_DEVICE		0x0220
#define PID_DEVICE		0x1000


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND	gListbox;

libusb_context * ctx = NULL;
const libusb_version * pLibVersion;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MyUsbProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void				ListDevices(HWND hDlg);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOGUSB), NULL, MyUsbProc);

#if 0
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYUSB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYUSB));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
#endif

	return 1;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYUSB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYUSB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void PrintVersion(HWND hDlg)
{
	TCHAR msg[300];
	wsprintf( msg, L"Lib %d.%d - %S", pLibVersion->major, pLibVersion->minor, pLibVersion->describe);
	ListBox_AddString(GetDlgItem(hDlg, IDC_USBLIST), msg);
}


TCHAR* GetClassDescription(int nClass)
{
	TCHAR desc[100];

	switch (nClass) {
	case LIBUSB_CLASS_PER_INTERFACE: return L"Per interface"; break;
	case LIBUSB_CLASS_COMM: return L"Communication"; break;
	case LIBUSB_CLASS_HID: return L"HID"; break;
	case LIBUSB_CLASS_HUB: return L"HUB"; break;
	default:
		wsprintf(desc, L"%d", nClass);
		return desc;
		break;
	}
}



void ListDevices(HWND hDlg)
{
	TCHAR msg[300];
	libusb_device **ppDevicesList;

	int nDevices = libusb_get_device_list( ctx, &ppDevicesList);

	wsprintf( msg, L"Found %d devices", nDevices);
	ListBox_AddString( gListbox, msg);
	
	for (int i = 0; i < nDevices; i++) {
		libusb_device *pDev = ppDevicesList[i];

		ListBox_AddString(gListbox, "");

		wsprintf(msg, L"Device %d:", i);
		ListBox_AddString(gListbox, msg);

		int nRow;
		libusb_device_descriptor devDescr;
		libusb_device_handle *handle;

		int ret = libusb_get_device_descriptor(pDev, &devDescr);
		if (ret == LIBUSB_SUCCESS) {

			wsprintf(msg, L"VID: %04X - PID: %04X", devDescr.idVendor, devDescr.idProduct);
			nRow = ListBox_AddString(gListbox, msg);
			ListBox_SetItemData(gListbox, nRow, pDev);

			ret = libusb_open(pDev, &handle);
			if (LIBUSB_SUCCESS == ret) {
				TCHAR wDescr[200];
				unsigned char string[200];

				if (devDescr.iManufacturer) {

					ret = libusb_get_string_descriptor_ascii(handle, devDescr.iManufacturer, string, sizeof(string));
					if (ret > 0)
						wsprintf(wDescr, L"%S - ", string);
					else
						wsprintf(wDescr, L"%04X - ", devDescr.idVendor);
				}
				else
					wsprintf(wDescr, L"%04X - ", devDescr.idVendor);

				if (devDescr.iProduct) {
					ret = libusb_get_string_descriptor_ascii(handle, devDescr.iProduct, string, sizeof(string));
					if (ret > 0)
						wsprintf(wDescr + wcslen(wDescr), L"%S", string);
					else
						wsprintf(wDescr + wcslen(wDescr), L"%04X", devDescr.idProduct);
				}
				else
					wsprintf(wDescr + wcslen(wDescr), L"%04X", devDescr.idProduct);
				ListBox_AddString(gListbox, wDescr);
				libusb_close(handle);
			}

			wsprintf(msg, L"MaxPacketSize: %d", devDescr.bMaxPacketSize0);
			ListBox_AddString(gListbox, msg);

			wsprintf(msg, L"NumConfig: %d", devDescr.bNumConfigurations);
			ListBox_AddString(gListbox, msg);

			wsprintf(msg, L"Class: %s Subclass: %d ", GetClassDescription(devDescr.bDeviceClass), devDescr.bDeviceSubClass);
			ListBox_AddString(gListbox, msg);
		}
		else {
			// err
			wsprintf(msg, L"Error getting descriptor %d", ret);
			ListBox_AddString(gListbox, msg);
		}
	}

	libusb_free_device_list(ppDevicesList, 1);
};


void OpenDevice(HWND hDlg)
{
	int idxsel = ListBox_GetCurSel(gListbox);
	libusb_device *pDev = (libusb_device*)ListBox_GetItemData(gListbox, idxsel);

	if (pDev == NULL) return;

	libusb_device_handle *hDev;
	int usbRet = libusb_open(pDev, &hDev);
	if (usbRet == 0) {
		libusb_close(hDev);
	}
	else {
		TCHAR msg[200];
		switch (usbRet) {
		default:
		case LIBUSB_ERROR_ACCESS:
			wcscpy(msg, L"Error access");
			break;

			wsprintf(msg, L"Open error: %X", usbRet);
			break;
		}
		ListBox_AddString(gListbox, msg);
	}
}



// Message handler for USB box.
INT_PTR CALLBACK MyUsbProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		gListbox = GetDlgItem(hDlg, IDC_USBLIST);

		libusb_init(&ctx);
		libusb_set_option(ctx, LIBUSB_OPTION_USE_USBDK);

		pLibVersion = libusb_get_version();
		pLibVersion->major;
		PrintVersion(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			libusb_exit(ctx);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_OPENDEV) {
			libusb_device_handle *pHandle = libusb_open_device_with_vid_pid(ctx, VID_DEVICE, PID_DEVICE);
			OpenDevice(hDlg);
		}
		else if (LOWORD(wParam) == IDC_LISTDEVS) {
			ListDevices(hDlg);
		}
		break;
	}
	return (INT_PTR)FALSE;
}
