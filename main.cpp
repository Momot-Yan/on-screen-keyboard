
#include <iostream>
#include <windows.h>
#include <string>

#define BTN_COUNT 110
HWND g_btns[BTN_COUNT];
const int ROW_COUNT = 6;
// Количество кнопок в каждом из 6 рядов:
const int ROWS[ROW_COUNT] = { 21, 21, 19, 17, 17, 15 }; 
const wchar_t* BUTTON_LABELS[BTN_COUNT] = {
    // Ряд 1 (21 кнопка): Мультимедиа + Esc + F1-F12 + Системные
    L"Mute", L"Vol-", L"Vol+", L"Web", L"Calc", L"Esc", L"F1", L"F2", L"F3", L"F4", L"F5", L"F6", L"F7", L"F8", L"F9", L"F10", L"F11", L"F12", L"PrtSc", L"ScrLk", L"Pause",

    // Ряд 2 (21 кнопка): Основной цифровой ряд + Управление + NumPad (верх)
    L"ё", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"-", L"=", L"Backspace", L"Ins", L"Home", L"PgUp", L"Num", L"/", L"*", L"-",

    // Ряд 3 (19 кнопок): Tab + Буквы Й-Ъ + \ + Управление + NumPad 7-9
    L"Tab", L"й", L"ц", L"у", L"к", L"е", L"н", L"г", L"ш", L"щ", L"з", L"х", L"ъ", L"\\", L"Del", L"End", L"PgDn", L"7", L"8", L"9", L"+",

    // Ряд 4 (17 кнопок): Caps + Буквы Ф-Э + Enter + NumPad 4-6
    L"Caps", L"ф", L"ы", L"в", L"а", L"п", L"р", L"о", L"л", L"д", L"ж", L"э", L"Enter", L"4", L"5", L"6", L"Enter ",

    // Ряд 5 (17 кнопок): LShift + Буквы Я-Ю + . + RShift + Стрелка Вверх + NumPad 1-3
    L"Shift", L"я", L"ч", L"с", L"м", L"и", L"т", L"ь", L"б", L"ю", L".", L"Shift ", L"↑", L"1", L"2", L"3", L"Enter  ",

    // Ряд 6 (15 кнопок): Модификаторы + Пробел + Стрелки + NumPad 0 и Точка
    L"Ctrl", L"Win", L"Alt", L"               Space               ", L"Alt", L"Win", L"Menu", L"Ctrl", L"←", L"↓", L"→", L"      0      ", L"."
};


///////////////////




///////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int);




///////////////////



///////////////////




///////////////////




///////////////////




///////////////////




///////////////////





///////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	static int client_w = 300, client_h = 600;

	switch (msg) {
        case WM_CREATE: {
			// Создаём 70 кнопок(уч.эскейп, дел,бэкспэйс, таб, ВИН, ЭфЭн, стрелки, райттаб, райтальт/ райтцтрл) без позиционирования (временно)
			for (int i = 0; i < BTN_COUNT; ++i) {
                // Размер буфера должен вмещать строку и завершающий ноль (например, "69" + '\0' = 3 символа)
                wchar_t txt[4];
                // Создаём кнопки с буквами русского алфавита

                if (i < 35) {
                    // Для первых 35 кнопок берем буквы= 33 + "\" + "."
                    txt[0] = RUS_LAYOUT[i];
                    txt[1] = L'\0';
                }
                else {
                    // Для остальных кнопок записываем индекс в виде строки
                    swprintf(txt, sizeof(txt) / sizeof(wchar_t), L"%d", i);
                }
                g_btns[i] = CreateWindow(L"BUTTON", txt,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    0, 0, 0, 0, hwnd, (HMENU)(INT_PTR)i, NULL, NULL);
			}
			return 0;
		}
		case WM_GETMINMAXINFO: {
			RECT r = { 0, 0, 100, 200 };
			AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
			((MINMAXINFO*)lp)->ptMinTrackSize.x = r.right - r.left;
			((MINMAXINFO*)lp)->ptMinTrackSize.y = r.bottom - r.top;
			return 0;
		}
        case WM_SIZE: {
            client_w = LOWORD(lp);
            client_h = HIWORD(lp);
            int top_h = client_h / 4;
            int btn_area_top = 1 + top_h;
            int btn_area_h = client_h - btn_area_top - 1;
            int btn_area_w = client_w - 2;

            int row_start = 0;
            for (int row = 0; row < 5/* ROWS.length */ ; ++row) {
                int btns_in_row = ROWS[row];
                int btn_w = btn_area_w / btns_in_row;
                int btn_h = btn_area_h / 5;
                int y = btn_area_top + row * btn_h;

                for (int col = 0; col < btns_in_row; ++col) {
                    int x = 1 + col * btn_w;
                    MoveWindow(g_btns[row_start + col], x, y, btn_w, btn_h, TRUE);
                }
                row_start += btns_in_row;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc; GetClientRect(hwnd, &rc);
            int w = rc.right, h = rc.bottom;
            int top_h = h / 4;

            HBRUSH bg = (HBRUSH)(COLOR_WINDOW + 1);
            FillRect(hdc, &rc, bg);

            HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HGDIOBJ old_pen = SelectObject(hdc, pen);
            MoveToEx(hdc, 0, 0, NULL); LineTo(hdc, w, 0);
            MoveToEx(hdc, 0, 0, NULL); LineTo(hdc, 0, h);
            MoveToEx(hdc, w - 1, 0, NULL); LineTo(hdc, w - 1, h);
            MoveToEx(hdc, 0, h - 1, NULL); LineTo(hdc, w, h - 1);
            SelectObject(hdc, old_pen);
            DeleteObject(pen);

            RECT top_rc = { 1, 1, w - 1, 1 + top_h };
            HBRUSH top_brush = CreateSolidBrush(RGB(220, 220, 220));
            FillRect(hdc, &top_rc, top_brush);
            DeleteObject(top_brush);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

	return DefWindowProc(hwnd, msg, wp, lp);
}


///////////////////

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"RawApp";
    RegisterClass(&wc);

    RECT r = { 0, 0, 420, 200 };
    // // usin' style WS_POPUPWINDOW instead of WS_OVERLAPPEDWINDO to remove the Windows' title bar of the window but still keep the window borders so that AdjustWindowRect works correctly.
    AdjustWindowRect(&r, WS_POPUPWINDOW | WS_CAPTION, FALSE);

    // now -- using CreateWindowEx +providing extndd style WS_EX_NOACTIVATE + WS_EX_TOPMOST
    HWND hwnd = CreateWindowEx(
        WS_EX_NOACTIVATE | WS_EX_TOPMOST,
        L"RawApp", L"70 Buttons",
        WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top,
        NULL, NULL, hInst, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}


///////////////////




///////////////////




///////////////////




///////////////////




///////////////////




///////////////////




///////////////////


