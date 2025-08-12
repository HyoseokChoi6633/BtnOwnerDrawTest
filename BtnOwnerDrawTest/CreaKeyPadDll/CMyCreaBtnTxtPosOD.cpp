#include "pch.h"
#include "CMyCreaBtnTxtPosOD.h"

#include "CMyCreaBtnTxtPosOD.h"

// 생성자(초기화 담당)
CMyCreaBtnTxtPosOD::CMyCreaBtnTxtPosOD() :
    m_hWndBtn(nullptr),
    m_iIDS_BTN(0),
    m_iPrtPos(0),
    m_iPadding(5)
{
    m_sizeTxtSize.cx = 0;
    m_sizeTxtSize.cy = 0;

    srand(time(NULL));
}

// 소멸자
CMyCreaBtnTxtPosOD::~CMyCreaBtnTxtPosOD()
{
    // 만약 클래스에서 동적 할당된 개체가 있다면 여기서 헤재해 주어도 된다.

    // 버튼에 사용한 폰트 핸들이 있다면 해제한다.
    // 스마트 포인터 적용 윈도우 핸들 반환
    if (m_hFontBtn) {
        m_hFontBtn.reset(nullptr);
    }
}

bool CMyCreaBtnTxtPosOD::CreateBtn(LPCTSTR pcszBtnTxt, PRECT prtBtnPos, HWND hWndParent, int iIDS_BTN, HINSTANCE hInst, int iPrtPos, int iPadding)
{
    bool bRetVal = false;
    RECT rtBtnPos;

    m_iIDS_BTN = iIDS_BTN;

    m_hFontBtn = nullptr;

    SetBtnTxtPos(iPrtPos);

    if (!prtBtnPos) {
        rtBtnPos.left = 0;
        rtBtnPos.top = 0;
        rtBtnPos.right = 0;
        rtBtnPos.bottom = 0;
    }
    else {
        rtBtnPos.left = prtBtnPos->left;
        rtBtnPos.top = prtBtnPos->top;
        rtBtnPos.right = prtBtnPos->right;
        rtBtnPos.bottom = prtBtnPos->bottom;
    }

    int iBtnWidth = rtBtnPos.right - rtBtnPos.left;
    int iBtnHeight = rtBtnPos.bottom - rtBtnPos.top;

    if (iBtnWidth < 1) {
        iBtnWidth = 50;
    }

    if (iBtnHeight < 1) {
        iBtnHeight = 50;
    }

    if (!m_hWndBtn) {
        m_hWndBtn = CreateWindow(_T("button"), pcszBtnTxt, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
            rtBtnPos.left, rtBtnPos.top, iBtnWidth, iBtnHeight,
            hWndParent, (HMENU)m_iIDS_BTN, hInst, nullptr);
    }

    if (m_hWndBtn) {
        bRetVal = true;
    }

    CalcPadding(iPadding);

    return bRetVal;
}

bool CMyCreaBtnTxtPosOD::OnDrawItem(LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

    if (!m_hWndBtn) {
        return false;
    }

    // 해당 오너 드로우 버튼인지 확인
    if (lpDIS->CtlID == m_iIDS_BTN)
    {
        HDC hDC = lpDIS->hDC;
        RECT rtBtn = lpDIS->rcItem;

        // 1. 버튼에 할당된 글꼴 가져오기 (매번 DrawItem에서 가져와야 함)
        HFONT hFontNowBtn = (HFONT)SendMessage(lpDIS->hwndItem, WM_GETFONT, 0, 0);

        // 2. GdiObjectSelector를 사용하여 폰트 선택 및 자동 복원
        // 이 객체가 스코프를 벗어나면 자동으로 SelectObject(hdc, hOldFont)가 호출됩니다.
        GdiObjectSelector fontSelector(hDC, hFontNowBtn); // hFont가 nullptr이면 SelectObject는 아무것도 안 함

        COLORREF clrBG; // 버튼 배경색
        COLORREF clrTxt = RGB(0, 0, 0); // 기본 텍스트 색상 (검정)

        // 1. 버튼 상태에 따른 색상 결정
        if (lpDIS->itemState & ODS_SELECTED) // 버튼이 눌린 상태
        {
            // 운영체제 기본 버튼 그림자 색상 사용
            clrBG = GetSysColor(COLOR_ACTIVEBORDER);
            // 텍스트 색상을 강조 배경에 어울리게 변경할 수도 있습니다.
            clrTxt = GetSysColor(COLOR_HIGHLIGHTTEXT);
        }
        else if (lpDIS->itemState & ODS_HOTLIGHT) // 마우스 오버 상태 (클릭은 아님)
        {
            // 마우스 오버 시 사용할 색상
            // 일반적으로 시스템 강조색 (COLOR_HIGHLIGHT) 또는 약간 밝은 색상을 사용
            clrBG = GetSysColor(COLOR_HIGHLIGHT); // 강조 배경색
            clrTxt = GetSysColor(COLOR_HIGHLIGHTTEXT); // 강조 배경에 맞는 텍스트 색상
        }
        else if (lpDIS->itemState & ODS_DISABLED) // 버튼이 비활성화된 상태
        {
            clrBG = GetSysColor(COLOR_BTNFACE); // 기본 배경색
            clrTxt = GetSysColor(COLOR_GRAYTEXT); // 회색 텍스트
        }
        else if (lpDIS->itemState & ODS_FOCUS) // 버튼이 포커스된 상태 (눌리지는 않음)
        {
            // 운영체제 강조 색상 사용 (포커스 배경색)
            clrBG = GetSysColor(COLOR_BTNFACE);
            clrTxt = GetSysColor(COLOR_BTNTEXT); // 강조 배경에 맞는 텍스트 색상
        }
        else // 기본 상태 (마우스 오버나 비활성화 등은 별도 처리)
        {
            // 운영체제 기본 버튼 표면색 사용
            clrBG = GetSysColor(COLOR_BTNFACE);
            clrTxt = GetSysColor(COLOR_BTNTEXT); // 회색 텍스트
        }

        // 2. 배경 채우기
        UniqueHBrush hBrush = UniqueHBrush(CreateSolidBrush(clrBG));
        FillRect(hDC, &rtBtn, (HBRUSH)hBrush.get());
        hBrush.reset();

        // 3. 테두리 그리기 (눌린 상태에 따라 테두리 모양 변경)
        if (lpDIS->itemState & ODS_SELECTED) {
            DrawEdge(hDC, &rtBtn, BDR_SUNKENOUTER, BF_RECT);    // 안으로 들어간 모양
        }
        else {
            DrawEdge(hDC, &rtBtn, BDR_RAISEDINNER, BF_RECT); // 튀어나온 모양
        }

        // 4. 텍스트 그리기 (숫자 위치 동적 계산 로직 포함)
        tstring strBtnTxt = GetBtnTxt();

        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, clrTxt); // 결정된 텍스트 색상 사용

        // 5. 포커스 사각형 그리기 (ODS_FOCUS 상태에서 점선 사각형)
        // ODS_FOCUS 상태일 때 이미 배경색을 변경했다면, 포커스 사각형은 선택 사항입니다.
        if (lpDIS->itemState & ODS_FOCUS && !(lpDIS->itemState & (ODS_SELECTED | ODS_HOTLIGHT))) // 눌린 상태가 아니면서 포커스일 때
        {
            DrawFocusRect(hDC, &rtBtn); // 버튼 영역에 포커스 사각형 그리기
        }

        // 6. 비활성화된 버튼 처리 (선택 사항)
        if (lpDIS->itemState & ODS_DISABLED)
        {
            // 비활성화된 버튼은 보통 회색으로 그리거나 흐리게 표현합니다.
            SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT)); // 회색 텍스트
            DrawState(hDC, NULL, NULL, (LPARAM)strBtnTxt.c_str(), 0, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, DST_TEXT | DSS_DISABLED);
        }

        // 9등분 또는 랜덤 위치 계산 로직을 여기에 삽입
        // ... (이전 답변에서 논의된 위치 계산 코드) ...
        RECT effectiveRect = { rtBtn.left + m_iPadding, rtBtn.top + m_iPadding, rtBtn.right - m_iPadding, rtBtn.bottom - m_iPadding };

        const int DIV3 = 3;
        int cellWidth = (effectiveRect.right - effectiveRect.left) / DIV3;
        int cellHeight = (effectiveRect.bottom - effectiveRect.top) / DIV3;

        int cellRow = m_iPrtPos / DIV3;
        int cellCol = m_iPrtPos % DIV3;

        // 예시: 항상 첫 번째 셀 (인덱스 0)에 고정
        RECT targetCellRect;
        targetCellRect.left = effectiveRect.left + (cellCol * cellWidth);
        targetCellRect.top = effectiveRect.top + (cellRow * cellHeight);
        targetCellRect.right = targetCellRect.left + cellWidth;
        targetCellRect.bottom = targetCellRect.top + cellHeight;

        // 유효 영역의 너비와 높이
        int validWidth = targetCellRect.right - targetCellRect.left;
        int validHeight = targetCellRect.bottom - targetCellRect.top;

        int iCorWidth = 0;
        int iCorHeight = 0;

        // 숫자가 유효 영역 안에 들어갈 수 있는지 확인
        // 이 조건이 충족되어야만 숫자의 랜덤 위치를 계산할 수 있다.
        if (validWidth < m_sizeTxtSize.cx) {
            iCorWidth = abs(m_sizeTxtSize.cx - validWidth);
        }

        if (validHeight < m_sizeTxtSize.cy) {
            iCorHeight = abs(m_sizeTxtSize.cy - validHeight);
        }

        // 오른쪽 너비 벗어남 처리
        switch (cellCol) {
        case 2:
            targetCellRect.right -= iCorWidth;
            targetCellRect.left = targetCellRect.right - m_sizeTxtSize.cx;
            break;
        default:
            // targetCellRect.right += m_sizeTxtSize.cx / 2;
            break;
        }

        // 바닥 벗어남 처리
        switch (cellRow) {
        case 2:
            targetCellRect.bottom -= iCorHeight;
            targetCellRect.top = targetCellRect.bottom - m_sizeTxtSize.cy;
            break;
        default:
            break;
        }

        // 벗어남 처리후 왼쪽이 음수 일때
        if (targetCellRect.left < 0) {
            targetCellRect.left = m_iPadding;
            targetCellRect.right = targetCellRect.left + m_sizeTxtSize.cx;
        }

        // 벗어남 처리후 위쪽이 음수 일때
        if (targetCellRect.top < 0) {
            targetCellRect.top = m_iPadding;
            targetCellRect.bottom = targetCellRect.top + m_sizeTxtSize.cy;
        }

        if (targetCellRect.left + m_sizeTxtSize.cx > effectiveRect.right) {
            targetCellRect.left -= (targetCellRect.left + m_sizeTxtSize.cx - effectiveRect.right);
        }

        if (targetCellRect.top + m_sizeTxtSize.cy > effectiveRect.bottom) {
            targetCellRect.bottom -= (targetCellRect.top + m_sizeTxtSize.cy - effectiveRect.bottom);
        }

        // WinAPI DrawText 함수 사용
        // 문자열 출력이 짤리 때가 있어 TextOut으로 대체 했다.
        //::DrawText(hDC, strBtnTxt.c_str(), -1, &targetCellRect, DT_SINGLELINE);

        TextOut(hDC, targetCellRect.left, targetCellRect.top, strBtnTxt.c_str(), strBtnTxt.size());

        // 스마트 포인터로 HFONT 를 이전상태로 복원
        fontSelector.reset();

        return true;
    }

    return false;
}

HWND CMyCreaBtnTxtPosOD::GetHWndBtn()
{
    return m_hWndBtn;
}

tstring CMyCreaBtnTxtPosOD::GetBtnTxt()
{
    int nLength = GetWindowTextLength(m_hWndBtn);

    if (nLength == 0) {
        return _T(""); // 텍스트가 없으면 빈 문자열 반환
    }

    // 2. 종료 null 문자를 포함하여 버퍼 생성(nLength + 1)
    // std::vector는 RAII(Resource Acquisition Is Initialization)를 따르므로,
    // 함수 범위를 벗어나면 자동으로 메모리가 해제됩니다.
    std::vector<TCHAR> buffer(nLength + 1);

    // 3. GetWindowText 호출하여 버퍼에 텍스트 복사
    // buffer.data()는 vector 내부 버퍼의 첫 번째 요소에 대한 포인터를 반환합니다.
    GetWindowText(m_hWndBtn, buffer.data(), nLength + 1);

    // 4. std::basic_string (tstring) 객체로 변환하여 반환
    return tstring(buffer.data());
}

void CMyCreaBtnTxtPosOD::SetBtnTxt(tstring strBtnTxt)
{
    SetWindowText(m_hWndBtn, strBtnTxt.c_str());

    CalcPadding(m_iPadding);
}

void CMyCreaBtnTxtPosOD::SetBtnTxtPos(int iPrtPos)
{
    if (iPrtPos == 0) {
        m_iPrtPos = rand() % 9;
    }
    else {
        m_iPrtPos = iPrtPos - 1;

        if (m_iPrtPos < 0 || m_iPrtPos > 8) {
            m_iPrtPos = 0; // 혹시 모를 잘못된 값에 대한 기본값
        }
    }

    InvalidateRect(m_hWndBtn, nullptr, true);
}

void CMyCreaBtnTxtPosOD::SetBtnFontForHeight(int& refiHeight)
{
    HFONT hFontBtn = nullptr;
    HFONT hFontBtnCreate = nullptr;

    // LOGFONT 구조체 선언 및 초기화
    LOGFONT logFont;

    RECT rtBtnClient;
    int iFontHeight;

    if (m_hWndBtn == nullptr) {
        return;
    }

    hFontBtn = (HFONT)SendMessage(m_hWndBtn, WM_GETFONT, 0, 0);

    if (hFontBtn == nullptr) {
        NONCLIENTMETRICS ncm;
        ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
        ncm.cbSize = sizeof(NONCLIENTMETRICS);

        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);

        memcpy_s(&logFont, sizeof(logFont), &ncm.lfMessageFont, sizeof(logFont));
    }
    else {
        GetObject(hFontBtn, sizeof(logFont), &logFont);
    }

    GetClientRect(m_hWndBtn, &rtBtnClient);

    // 버튼의 클라이언트 영역과 비교해서
    // 높이를 적당하게 설정한다.
    // 입력한 값이 적당하면 바로 넣고
    // 아니면 조절한다.

    iFontHeight = rtBtnClient.bottom - rtBtnClient.top;
    iFontHeight /= 3;

    if (refiHeight == 0) {
        refiHeight = iFontHeight;
    }
    else if (refiHeight > iFontHeight) {
        refiHeight = iFontHeight;
    }

    if (logFont.lfHeight > 0) {
        logFont.lfHeight = refiHeight;
    }
    else {
        logFont.lfHeight = -refiHeight;
    }

    hFontBtnCreate = CreateFontIndirect(&logFont);

    if (m_hFontBtn) {
        m_hFontBtn.reset();
    }

    m_hFontBtn.reset(hFontBtnCreate);

    SendMessage(m_hWndBtn, WM_SETFONT, (WPARAM)m_hFontBtn.get(), MAKELONG(TRUE, 0));

    CalcPadding(m_iPadding);
}

void CMyCreaBtnTxtPosOD::CalcPadding(int iPadding)
{
    RECT rtBtnTxt;
    tstring strBtnTxt;
    HFONT hFontOld = nullptr;

    UniqueHdc hDC(
        ::GetDC(m_hWndBtn), HdcDeleter(m_hWndBtn) // GetDC() 호출 및 사용자 정의 삭제자 전달
    );

    strBtnTxt = GetBtnTxt();

    if (hDC) {
        if (m_hFontBtn) {
            // 2. GdiObjectSelector를 사용하여 폰트 선택 및 자동 복원
            // 이 객체가 스코프를 벗어나면 자동으로 SelectObject(hdc, hOldFont)가 호출됩니다.
            GdiObjectSelector fontSelector((HDC)hDC.get(), m_hFontBtn.get()); // hFont가 nullptr이면 SelectObject는 아무것도 안 함

            ::GetTextExtentPoint32((HDC)hDC.get(), strBtnTxt.c_str(), strBtnTxt.size(), &m_sizeTxtSize);

            fontSelector.reset();
        }

        hDC.reset();
    }

    if (m_sizeTxtSize.cx == 0 && m_sizeTxtSize.cy == 0) {
        return;
    }

    // 여백값도 유효성있게 조절해야 함.
    int iNessWidth = m_sizeTxtSize.cx + iPadding * 2;
    int iNessHeight = m_sizeTxtSize.cy + iPadding * 2;

    int iPaddingWidth = iPadding;
    int iPaddingHeight = iPadding;

    GetClientRect(m_hWndBtn, &rtBtnTxt);

    int iBtnWidth = rtBtnTxt.right - rtBtnTxt.left;
    int iBtnHeight = rtBtnTxt.bottom - rtBtnTxt.top;

    if (iNessWidth > iBtnWidth) {
        iPaddingWidth = iPadding - (iNessWidth - iBtnWidth);
        if (iPaddingWidth < 0) {
            iPaddingWidth = 0;
        }
    }

    if (iNessHeight > iBtnHeight) {
        iPaddingHeight = iPadding - (iNessHeight - iBtnHeight);
        if (iPaddingHeight < 0) {
            iPaddingHeight = 0;
        }
    }

    m_iPadding = min(iPaddingWidth, iPaddingHeight);
}
