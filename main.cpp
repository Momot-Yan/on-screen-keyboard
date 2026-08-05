
#include <iostream>
#include <windows.h>

#define BTN_COUNT 70
HWND g_btns[BTN_COUNT];
const int ROWS[5] = { 14, 14, 14, 14, 14 }; // кнопок в р€дах, 5*14=70
const wchar_t* RUS_LAYOUT = LR"(йцукенгшщзхъ\фывапролджэ€чсмитьбю.Є)";


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



int main()
{
	return 0;
}



///////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	static int client_w = 300, client_h = 600;

	switch (msg) {
        case WM_CREATE: {
			// —оздаЄм 70 кнопок(уч.эскейп, дел,бэкспэйс, таб, ¬»Ќ, ЁфЁн, стрелки, райттаб, райтальт/ райтцтрл) без позиционировани€ (временно)
			for (int i = 0; i < BTN_COUNT; ++i) {
                // –азмер буфера должен вмещать строку и завершающий ноль (например, "69" + '\0' = 3 символа)
                wchar_t txt[4];
                // —оздаЄм кнопки с буквами русского алфавита

                if (i < 35) {
                    // ƒл€ первых 35 кнопок берем буквы= 33 + "\" + "."
                    txt[0] = RUS_LAYOUT[i];
                    txt[1] = L'\0';
                }
                else {
                    // ƒл€ остальных кнопок записываем индекс в виде строки
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

    RECT r = { 0, 0, 300, 600 };
    // usin' style WS_POPUPWINDOW вместо WS_OVERLAPPEDWINDOW, чтобы убрать лишний фокусный заголовок Windows,but save bordrs 4 AdjustWindowRect work properly.
    AdjustWindowRect(&r, WS_POPUPWINDOW | WS_CAPTION, FALSE);

    // now -- using right CreateWindowEx +providing extndd style WS_EX_NOACTIVATE + WS_EX_TOPMOST
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


