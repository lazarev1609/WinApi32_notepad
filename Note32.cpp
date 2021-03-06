// WindowsProject3.cpp : Определяет точку входа для приложения.
//

#include "pch.h"
#include "WindowsProject3.h"
#include <>
#include <commdlg.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <commctrl.h>
#include <stdio.h>

#define MAX_LOADSTRING 100
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр, дескриптор экземпляра приложения 
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND but1;
HWND but2;
HWND but3;
HWND edit;
HWND toolbar;

int nxPos = 0;
//char cBufferText[1000]; 
char *cBufferText1=0;
char *cBufferText2 = 0;
HMENU hFileMenu, hHelpMenu, hMenu;
using namespace std;


HANDLE hFile;
DWORD lpNumberOfBytesRead;//Кол-во прочитаных байтов, если 0 то уже конец файла

// Хендл окна
HWND hWnd;
// Идентификатор окна (описывается глобально!)
HWND hCombo;
// Идентификатор списка (описывается глобально!)

//typedef struct tagMINMAXINFO
//{
//	POINT ptReserved;
//	POINT ptMaxSize;
//	POINT ptMaxPosition;
//	POINT ptMinTrackSize;
//	POINT ptMaxTrackSize;
//} MINMAXINFO;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));//загрузка ускорителей

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
	
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;//горизонтальная и вертикальная перерисовка
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName  = szWindowClass;
	//Стандартная пиктограмма, которую можно загрузить функцией
		//LoadImage()
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	//HMENU hMenu;       // Menu
	//hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)); // Загрузка меню

	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,//создание главного окна
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
	   return FALSE;

  
  edit = CreateWindow(_T("EDIT"), _T(""),
	   WS_CHILD | WS_VISIBLE | WS_BORDER|ES_MULTILINE|WS_VSCROLL | WS_HSCROLL, 0,0,200,250,
	   hWnd, (HMENU)666, hInstance, 0);

  
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
  // DrawMenuBar(hWnd);
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
		int wmId;

		PAINTSTRUCT ps;
		
		HDC hdc;
		static TCHAR name[256] = _T("");//хранение имени открываемого файла 
		static OPENFILENAME file;//переменная типа структуры OPENFILENAME
		std::ifstream in;//Видимости. Для открытия файла
		std::ofstream out;//Для закрытия файла 
		static std::vector<std::string> v; //вектор для данных типа string
		std::vector<std::string>::iterator it;//
		std::string st;
		static int n, length, sx, sy, cx, iVscrollPos, iHscrollPos, COUNT, MAX_WIDTH;
		static SIZE size = { 8, 16 }; //Ширина и высота символа
		int y;
		
	//COLORREF colorText = RGB(255, 0, 0);//для цвета

    switch (message)
    {
	case WM_CREATE:
		
		file.lStructSize = sizeof(OPENFILENAME);
		file.hInstance = hInst;
		file.lpstrFilter = _T("Text\0*.txt");
		file.lpstrFile = name;
		file.nMaxFile = 256;
		file.lpstrInitialDir = _T(".\\");
		file.lpstrDefExt = _T("txt");
		//GetOpenFileName(&file);
		break;
		//{ // Создание туллбара, картинки брал с интернета и вставлял в ресурс с тулбаром
		//	TBBUTTON tbb[7];//Массив структур TBBUTTON
		//	ZeroMemory(&tbb, sizeof(tbb));//Обнулим весь массив структур
		//	tbb[0].iBitmap = 0;//Порядковый номер кнопки I
		//	tbb[0].idCommand = ID_32771;//Идентификатор кнопки 1
		//	tbb[0].fsState = TBSTATE_ENABLED;//Разрешить управление от кнопки 1
		//	tbb[1].iBitmap = 1;//Порядковый номер кнопки 2
		//	tbb[1].idCommand = ID_32772;//Идентификатор кнопки 2
		//	tbb[1].fsState = TBSTATE_ENABLED; //Разрешить управление от кнопки 2
		//	tbb[2].iBitmap = 2;// Порядковый номер кнопки 3
		//	tbb[2].idCommand = ID_32773;//Идентификатор кнопки 3
		//	tbb[2].fsState = TBSTATE_ENABLED; //Разрешить управление от кнопки 3
		//	tbb[3].iBitmap = 3;// Порядковый номер кнопки 4
		//	tbb[3].idCommand = ID_32777;//Идентификатор кнопки 4
		//	tbb[3].fsState = TBSTATE_ENABLED; //Разрешить управление от кнопки 4
		//	tbb[4].iBitmap = 4;// Порядковый номер кнопки 5
		//	tbb[4].idCommand = ID_32774;//Идентификатор кнопки 5
		//	tbb[4].fsState = TBSTATE_ENABLED; //Разрешить управление от кнопки 5
		//	tbb[5].iBitmap = 5;// Порядковый номер кнопки 6
		//	tbb[5].idCommand = ID_32776;//Идентификатор кнопки 6
		//	tbb[5].fsState = TBSTATE_ENABLED; //Разрешить управление от кнопки 6
		//	tbb[6].iBitmap = 6;// Порядковый номер кнопки 7
		//	tbb[6].idCommand = IDM_EXIT;//Идентификатор кнопки 7
		//	tbb[6].fsState = TBSTATE_ENABLED; //Разрешить управление от кнопки 7
		//	toolbar = CreateToolbarEx(hWnd, WS_CHILD | WS_BORDER | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
		//		-1, 3, hInst, IDR_TOOLBAR1, tbb, 7, 16, 16, 16, 16, sizeof(TBBUTTON)); //Создаем инструментальную панель
		//}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			
			case 32783://новый файл??
				/*if (!v.empty()) std::vector<std::string>().swap(v);
				n = length = 0;*/
				SetWindowText(edit, cBufferText1);
				//SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);
				InvalidateRect(hWnd, NULL, TRUE);
				break;  

			case 32784://открытие файла 
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				file.lpstrTitle = _T("Открыть файл для чтения");
				file.Flags = OFN_HIDEREADONLY;
				if (!GetOpenFileName(&file)) return 1;
				
				hFile = CreateFile(  //функция создания ANSI!!!последния буква А
					name, //это имя файла \\условно считаеться как \ но писать 2 иначе vs подумает что служ. команда
					GENERIC_READ | GENERIC_WRITE,  //в режим чтения GENERIC_WRITE то в запись 
					FILE_SHARE_READ, NULL,//либо 0 если сторонние пользователи не имеют доступ к файлу
					OPEN_EXISTING, //говорим что открываем файл УЖЕ СУЩЕСТВУЮЩИЙ КАК C:\MYFILES.TXT
					FILE_ATTRIBUTE_NORMAL, //Это обычный файл с атриьутом не архивный и т.п.
					NULL); //защита, есть отдельная структура 

				if (hFile == INVALID_HANDLE_VALUE)   //если ошибка то выходим INVALID_HANDLE_VALUE - некоторая переменная с некоторым значением типа #defain INVALID_HANDLE_VALUE Х..З
				{
					MessageBox(NULL, TEXT("Ошибка КЛИЕНТА! - неудалось открыть файл"), TEXT("Warning"), MB_OK);
					CloseHandle(hFile);
					return 0;	
				}
				DWORD sz1, sz2;

				sz1=GetFileSize(hFile, &sz2);
			
				cBufferText1 = (char *)malloc(sz1 + 1);
					ReadFile( //функция чтения данных
						hFile,
						cBufferText1, sz1, &lpNumberOfBytesRead, NULL);
					
				CloseHandle(hFile); //закрыть файл
				cBufferText1[lpNumberOfBytesRead] = 0;
				SetWindowText(edit, cBufferText1);
				free(cBufferText1);
				break;

			case 32791:    //сохранение файла готовый документ или в новый
				
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				file.lpstrTitle = _T("Сохранить файл как....");
				file.Flags = OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT;;
				if (GetSaveFileName(&file)) return 1;
				{
					FILE *f;
					DWORD size1, size2;
					size1 = GetWindowTextLength(edit);
					cBufferText2 = (char *)malloc(size1 + 1);
					GetWindowText(edit, cBufferText2, size1 + 1);
					fwrite(cBufferText2,size+1, 1, f);
					free(cBufferText2);
				}

				DWORD size1,size2;
				size1 = GetWindowTextLength(edit);
				cBufferText2 = (char *)malloc(size1 + 1);
				GetWindowText(edit, cBufferText2, size1+1);
				//cBufferText2[lpNumberOfBytesRead] = 0;//конец файла
				//WriteFile(hFile, cBufferText2, size1, &size2, NULL);//запись в файл
				FILE *f;

				CloseHandle(hFile); //закрыть файл1
				free(cBufferText2);
			break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
             /*   break; */ 
					
			//case 123:
				
				//break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
			/*for (y = 0, it = v.begin() + iVscrollPos; it != v.end() && y < sy; ++it, y += size.cy)
				if (iHscrollPos < it->length())
					TabbedTextOutA(hdc, 0, y, it->data() + iHscrollPos, it->length() -
						iHscrollPos, 0, NULL, 0);
			EndPaint(edit, &ps);*/
        
        }
     break;
	/*case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: iVscrollPos--; break;
		case SB_LINEDOWN: iVscrollPos++; break;
		case SB_PAGEUP: iVscrollPos -= sy / size.cy; break;
		case SB_PAGEDOWN: iVscrollPos += sy / size.cy; break;
		case SB_THUMBPOSITION: iVscrollPos = HIWORD(wParam); break;
		}
		iVscrollPos = max(0, min(iVscrollPos, COUNT));
		if (iVscrollPos != GetScrollPos(hWnd, SB_VERT))
		{
			SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: iHscrollPos--; break;
		case SB_LINEDOWN: iHscrollPos++; break;
		case SB_PAGEUP: iHscrollPos -= 8; break;
		case SB_PAGEDOWN: iHscrollPos += 8; break;
		case SB_THUMBPOSITION: iHscrollPos = HIWORD(wParam); break;
		}
		iHscrollPos = max(0, min(iHscrollPos, MAX_WIDTH));
		if (iHscrollPos != GetScrollPos(hWnd, SB_HORZ))
		{
			SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;*/
	case WM_SIZE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		SetWindowPos(edit, 0, 0,0, x-20, y-100, 0);
		SetWindowPos(but1, 0, 0, y-50, 150, 50, 0);
		SetWindowPos(but2, 0, 150, y - 50, 150, 50, 0);
		SetWindowPos(but3, 0, 300, y - 50, 150, 50, 0);
		
	}
		break;
	case WM_GETMINMAXINFO: //Получили сообщение от Винды
	{
		MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
		POINT Min = { 800,400  };
		//POINT  Max = { 100, 100 };
		pInfo->ptMinTrackSize = Min; // Установили минимальный размер
		//pInfo->ptMaxTrackSize = Max; // Установили максимальный размер
		return 0;
	}
		
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
