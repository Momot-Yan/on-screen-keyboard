
#include <iostream>
#include <windows.h>
#include <string>

#define BTN_COUNT 110
HWND g_btns[BTN_COUNT];
const int ROW_COUNT = 6;
// grid density mappin': exact key cnt per each of the 6 physical rows
const int ROWS[ROW_COUNT] = { 21, 21, 19, 17, 17, 15 }; 
const wchar_t* BUTTON_LABELS[BTN_COUNT] = {
    // row 1 (21 btns): Media controls + Esc + F-keys + Sys mngmnt block
    L"Mute", L"Vol-", L"Vol+", L"Web", L"Calc", L"Esc", L"F1", L"F2", L"F3", L"F4", L"F5", L"F6", L"F7", L"F8", L"F9", L"F10", L"F11", L"F12", L"PrtSc", L"ScrLk", L"Pause",

    // row 2 (21 btns): Primary digit sequence + Core navigatn + Numpad ceilin'
    L"ё", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"-", L"=", L"Backspace", L"Ins", L"Home", L"PgUp", L"Num", L"/", L"*", L"-",

    // row 3 (19 btns): Tab + Cyrillic top deck + Direct slash + Nav block + Numpad 7-9
    L"Tab", L"й", L"ц", L"у", L"к", L"е", L"н", L"г", L"ш", L"щ", L"з", L"х", L"ъ", L"\\", L"Del", L"End", L"PgDn", L"7", L"8", L"9", L"+",

    // row 4 (17 btns): Caps Lock control + Cyrillic home deck + Dedicated Enter mappin'
    L"Caps", L"ф", L"ы", L"в", L"а", L"п", L"р", L"о", L"л", L"д", L"ж", L"э", L"Enter", L"4", L"5", L"6", L"Enter ",

    // row 5 (17 btns): Left shift + Cyrillic base deck + Modifiers + Right shift + Up arrow + Numpad 1-3
    L"Shift", L"я", L"ч", L"с", L"м", L"и", L"т", L"ь", L"б", L"ю", L".", L"Shift ", L"↑", L"1", L"2", L"3", L"Enter  ",

    // row 6 (15 btns): Lower modifiers + Calibrated spacebar + Arrow cluster + Numpad zero floor
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
            // generatin' 110 child window instances mapped directly to the raw label registry
            for (int i = 0; i < BTN_COUNT; ++i) {
                g_btns[i] = CreateWindow(L"BUTTON", BUTTON_LABELS[i],
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
		}        case WM_SIZE: {
            client_w = LOWORD(lp);
            client_h = HIWORD(lp);

            int top_h = client_h / 4;
            int btn_area_top = 1 + top_h;
            int btn_area_h = client_h - btn_area_top - 1;
            int btn_area_w = client_w - 2;

            int row_h = btn_area_h / ROW_COUNT;
            int global_idx = 0;

            // evaluating all 6 physical rows 2 prevent layout clippin'
            for (int row = 0; row < ROW_COUNT; ++row) {
                int btns_in_row = ROWS[row];

                // first pass: computing the aggregate Key Unit weight of the entire row segment
                double total_units = 0.0;
                for (int col = 0; col < btns_in_row; ++col) {
                    int idx = global_idx + col;
                    std::wstring label = BUTTON_LABELS[idx];

                    if (label.find(L"Space") != std::wstring::npos) total_units += 6.0;
                    else if (label == L"Backspace" || label == L"Shift" || label == L"Shift ") total_units += 2.0;
                    else if (label == L"Enter" || label == L"      0      ") total_units += 2.25;
                    else if (label == L"Tab" || label == L"Caps" || label == L"Ctrl") total_units += 1.5;
                    else total_units += 1.0;
                }

                double unit_w = (double)btn_area_w / total_units;
                double current_x = 1.0;
                int y = btn_area_top + row * row_h;

                // second pass: positioning indvdl child btns bas'd on calcltd proportions
                for (int col = 0; col < btns_in_row; ++col) {
                    int idx = global_idx + col;
                    std::wstring label = BUTTON_LABELS[idx];

                    double w_multiplier = 1.0;
                    if (label.find(L"Space") != std::wstring::npos) w_multiplier = 6.0;
                    else if (label == L"Backspace" || label == L"Shift" || label == L"Shift ") w_multiplier = 2.0;
                    else if (label == L"Enter" || label == L"      0      ") w_multiplier = 2.25;
                    else if (label == L"Tab" || label == L"Caps" || label == L"Ctrl") w_multiplier = 1.5;

                    int btn_w = (int)(w_multiplier * unit_w);

                    // preventing pixel gaps on the right edge due 2 float-2-int trunctn' when col == last_idx
                    if (col == btns_in_row - 1) {
                        btn_w = btn_area_w - (int)current_x + 1;
                    }

                    MoveWindow(g_btns[idx], (int)current_x, y, btn_w, row_h, TRUE);
                    current_x += btn_w;
                }
                global_idx += btns_in_row;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);
            int w = rc.right, h = rc.bottom;
            int top_h = h / 4;

            HBRUSH bg = (HBRUSH)(COLOR_WINDOW + 1);
            FillRect(hdc, &rc, bg);

            // drawing sharp boundary outline lines 4 the entire frame edge
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HGDIOBJ old_pen = SelectObject(hdc, pen);
            MoveToEx(hdc, 0, 0, NULL); LineTo(hdc, w, 0);
            MoveToEx(hdc, 0, 0, NULL); LineTo(hdc, 0, h);
            MoveToEx(hdc, w - 1, 0, NULL); LineTo(hdc, w - 1, h);
            MoveToEx(hdc, 0, h - 1, NULL); LineTo(hdc, w, h - 1);
            SelectObject(hdc, old_pen);
            DeleteObject(pen);

            // materializin' the separate gray top deck pane 4 suggestions info display
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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"RawApp";
    RegisterClass(&wc);

    RECT r = { 0, 0, 1200, 200 };
    // strippin' the OS title bar via WS_POPUPWINDOW while bypassin' default frame geometry
    AdjustWindowRect(&r, WS_POPUPWINDOW | WS_CAPTION, FALSE);

    // initlzn' CreateWindowEx w/ xtendd styles 2 enforce clean click-through immunity + absolute topmost dominance
    HWND hwnd = CreateWindowEx(
        WS_EX_NOACTIVATE | WS_EX_TOPMOST,
        L"RawApp", L"110 Buttons Keyboard",
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


