#include <stdio.h>
#include <windows.h>

#define ID_EDIT1 101
#define ID_EDIT2 102
#define ID_BTN_ADD 201
#define ID_BTN_SUB 202
#define ID_BTN_MUL 203
#define ID_BTN_DIV 204

COLORREF g_bgColor = RGB(0, 120, 215);
HBRUSH g_hBrush = NULL;

void ShowResult(HWND hwnd, double result) {
  char buf[64];
  sprintf(buf, "%g", result);
  MessageBox(hwnd, buf, "Result", MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam,
                         LPARAM lParam) {
  static HWND hEdit1, hEdit2, hLabel, hBtnAdd, hBtnSub, hBtnMul, hBtnDiv;
  switch (Message) {
  case WM_CREATE: {
    g_hBrush = CreateSolidBrush(g_bgColor);

    // Label
    hLabel = CreateWindow("STATIC", "Please input two numbers",
                          WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 10, 180, 30,
                          hwnd, NULL, NULL, NULL);
    SendMessage(hLabel, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                TRUE);
    // box 1
    hEdit1 = CreateWindow("EDIT", "",
                          WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, 30,
                          50, 140, 30, hwnd, (HMENU)ID_EDIT1, NULL, NULL);
    SendMessage(hEdit1, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                TRUE);
    // box 2
    hEdit2 = CreateWindow("EDIT", "",
                          WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, 30,
                          90, 140, 30, hwnd, (HMENU)ID_EDIT2, NULL, NULL);
    SendMessage(hEdit2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                TRUE);
    // +
    hBtnAdd = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE, 20, 140, 40,
                           40, hwnd, (HMENU)ID_BTN_ADD, NULL, NULL);
    SendMessage(hBtnAdd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                TRUE);
    // -
    hBtnSub = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE, 65, 140, 40,
                           40, hwnd, (HMENU)ID_BTN_SUB, NULL, NULL);
    SendMessage(hBtnSub, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                TRUE);
    // *
    hBtnMul = CreateWindow("BUTTON", "*", WS_CHILD | WS_VISIBLE, 110, 140, 40,
                           40, hwnd, (HMENU)ID_BTN_MUL, NULL, NULL);
    SendMessage(hBtnMul, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                TRUE);
    // /
    hBtnDiv = CreateWindow("BUTTON", "/", WS_CHILD | WS_VISIBLE, 155, 140, 40,
                           40, hwnd, (HMENU)ID_BTN_DIV, NULL, NULL);
    SendMessage(hBtnDiv, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                TRUE);
    break;
  }
  case WM_COMMAND: {
    int wmId = LOWORD(wParam);
    if (wmId == ID_BTN_ADD || wmId == ID_BTN_SUB || wmId == ID_BTN_MUL ||
        wmId == ID_BTN_DIV) {
      char buf1[64], buf2[64];
      GetWindowText(GetDlgItem(hwnd, ID_EDIT1), buf1, 63);
      GetWindowText(GetDlgItem(hwnd, ID_EDIT2), buf2, 63);
      double n1, n2;
      if (1 != sscanf_s(buf1, "%lf", &n1) || sscanf_s(buf2, "%lf", &n2) != 1) {
        MessageBox(hwnd, "Please input valid numbers", "Error",
                   MB_OK | MB_ICONERROR);
        break;
      }
      double result = 0;
      switch (wmId) {
      case ID_BTN_ADD:
        result = n1 + n2;
        break;
      case ID_BTN_SUB:
        result = n1 - n2;
        break;
      case ID_BTN_MUL:
        result = n1 * n2;
        break;
      case ID_BTN_DIV:
        if (n2 == 0) {
          MessageBox(hwnd, "Cannot divide by zero", "Error",
                     MB_OK | MB_ICONERROR);
          break;
        }
        result = n1 / n2;
        break;
      }
      if (!(wmId == ID_BTN_DIV && n2 == 0))
        ShowResult(hwnd, result);
    }
    break;
  }
  case WM_CTLCOLOREDIT: {
    HDC hdc = (HDC)wParam;
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    return (LRESULT)GetStockObject(WHITE_BRUSH);
  }
  case WM_CTLCOLORSTATIC:
  case WM_CTLCOLORBTN: {
    HDC hdc = (HDC)wParam;
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    return (LRESULT)g_hBrush;
  }
  case WM_ERASEBKGND: {
    HDC hdc = (HDC)wParam;
    RECT rc;
    GetClientRect(hwnd, &rc);
    FillRect(hdc, &rc, g_hBrush);
    return 1;
  }
  case WM_GETMINMAXINFO: {
    MINMAXINFO *mmi = (MINMAXINFO *)lParam;
    mmi->ptMinTrackSize.x = 225;
    mmi->ptMinTrackSize.y = 250;
    mmi->ptMaxTrackSize.x = 225;
    mmi->ptMaxTrackSize.y = 250;
    break;
  }
  case WM_DESTROY: {
    if (g_hBrush) {
      DeleteObject(g_hBrush);
      g_hBrush = NULL;
    }
    PostQuitMessage(0);
    break;
  }
  default:
    return DefWindowProc(hwnd, Message, wParam, lParam);
  }
  return 0;
}
/* The 'main' function of Win32 GUI programs: this is where execution starts */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASSEX wc; /* A properties struct of our window */
  HWND hwnd;     /* A 'HANDLE', hence the H, or a pointer to our window */
  MSG msg;       /* A temporary location for all messages */

  (void)hPrevInstance;
  (void)lpCmdLine;

  /* zero out the struct and set the stuff we want to modify */
  memset(&wc, 0, sizeof(wc));
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  /* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking
   * it */
  wc.hbrBackground = NULL;
  wc.lpszClassName = "WindowClass";
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&wc)) {
    MessageBox(NULL, "Window Registration Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  hwnd = CreateWindowEx(
      0, "WindowClass", "My Calculator",
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, /* x */
      CW_USEDEFAULT,                                          /* y */
      225, /* width */ 250, /* height */ NULL, NULL, hInstance, NULL);

  if (hwnd == NULL) {
    MessageBox(NULL, "Window Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  /*
                This is the heart of our program where all input is processed
     and sent to WndProc. Note that GetMessage blocks code flow until it
     receives something, so this loop will not produce unreasonably high CPU
     usage
        */
  while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
    TranslateMessage(&msg); /* Translate key codes to chars if present */
    DispatchMessage(&msg);  /* Send it to WndProc */
  }
  return (int)msg.wParam;
}
