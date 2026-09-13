// BtnODView.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
// 2025-0709
// 키 페드 버튼만 구현
// 버튼 클릭에 대한 이벤트 구현 예정
// 
// 2025-0711
// 키 페드 구현 완료
// 다음 예정은 CMy 클래스를 dll로 구현할 예정

// 2025-0713
// 스마트 포인터 사용 gdi 객체 자동 닫기 구현
// using을 이용 스마트 포인터 선언의 간략화 구현

// 2025-0714
// 키 페드 출력 dll 클래스로 작성 후
// App 이 dll을 이용해 키 페드 출력을 하게 만들었음.

// 2025-0717
// SpGdi.h 를 SSGdi.h 로 변경
// DeleteObject(); 자동화 에서 시스템 객체는 삭제 못하게 하는 방법 추가

#include "framework.h"
#include "BtnODView.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_BTNODVIEW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BTNODVIEW));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BTNODVIEW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_BTNODVIEW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //static CMyCreaBtnTxtPosOD arobjBtn[10];

    //int i;

    //TCHAR sztPrtNum[2] = { 0, };

    //HWND hWndBtn;

    //tstring strBtnTxt;

    static CreaKeyPad objKeyPad;

    POINT ptStart;
    SIZE szBtnSize;

    const int IDS_BTNS_FIRST = 1000;

    LPCTSTR lpctPass = nullptr;

    switch (message)
    {
    case WM_CREATE:
        // SetRect(&rtBtnPos, 50, 50, 50 + 50, 50 + 50);

        //for (i = 0; i < 10; i++) {
        //    wsprintf(sztPrtNum, _T("%d"), i);
        //    arobjBtn[i].CreateBtn(sztPrtNum, &rtBtnPos, hWnd, IDS_BTNS_FIRST + i, hInst, 0, 5);

        //    rtBtnPos.left += 50;
        //    rtBtnPos.right += 50;

        //    if ((i + 1) % 5 == 0) {
        //        rtBtnPos.left = 50;
        //        rtBtnPos.right = 100;

        //        rtBtnPos.top += 50;
        //        rtBtnPos.bottom += 50;
        //    }
        //}

        ptStart.x = 50;
        ptStart.y = 50;

        szBtnSize.cx = 110;
        szBtnSize.cy = 80;

        if (!objKeyPad.CreateKeyPad(hWnd, hInst, ptStart, szBtnSize, IDS_BTNS_FIRST, 5, 3, false, 4)) {
            MessageBox(hWnd, _T("키 페드 생성에 실패 했습니다."), _T("경고"), MB_OK);

            return false;
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            //if (wmId >= IDS_BTNS_FIRST && wmId <= IDS_BTNS_FIRST + 9) {
            //    strBtnTxt = arobjBtn[wmId - IDS_BTNS_FIRST].GetBtnTxt();
            //    MessageBox(hWnd, strBtnTxt.c_str(), _T("버튼확인"), MB_OK);

            //    bool bIsDigit = false;
            //    TCHAR chA = _T('A');
            //    TCHAR ch1 = _T('1');
            //    int iResult = 0;

            //    if (isdigit(strBtnTxt[0])) {
            //        bIsDigit = true;
            //    }

            //    strBtnTxt.resize(1024);

            //    if (bIsDigit) {
            //        iResult = _stprintf_s(&strBtnTxt[0], 1024, _T("%c%c"), chA + wmId - IDS_BTNS_FIRST, chA + wmId - IDS_BTNS_FIRST);
            //    }
            //    else {
            //        iResult = _stprintf_s(&strBtnTxt[0], 1024, _T("%c"), ch1 + wmId - IDS_BTNS_FIRST - 1);
            //    }
            //    
            //    strBtnTxt.resize(iResult);

            //    arobjBtn[wmId - IDS_BTNS_FIRST].SetBtnTxt(strBtnTxt);

            //    arobjBtn[wmId - IDS_BTNS_FIRST].SetBtnTxtPos(0);

            //    hWndBtn = arobjBtn[wmId - IDS_BTNS_FIRST].GetHWndBtn();
            //    if (hWndBtn) {
            //        SetFocus(hWndBtn);
            //    }
            //    
            //    break;
            //}

            if (objKeyPad.OnCommand(wmId, HIWORD(wParam), &lpctPass)) {
                if (lpctPass && _tcslen(lpctPass)) {

                    MessageBox(hWnd, lpctPass, _T("확인"), MB_OK);
                }
            }

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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DRAWITEM:
        //for (i = 0; i < 10; i++) {
        //    if (arobjBtn[i].OnDrawItem(lParam)) {
        //        return TRUE;
        //    }
        //}
        objKeyPad.OnDrawItem(lParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
