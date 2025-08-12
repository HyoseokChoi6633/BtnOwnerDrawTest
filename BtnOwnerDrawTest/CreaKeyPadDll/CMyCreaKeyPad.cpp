#include "pch.h"
#include "CMyCreaKeyPad.h"

namespace CreaKeyPad_Library
{
	CreaKeyPad::CreaKeyPad() :
		m_hWndStDispNum(nullptr),
		m_hWndCmbPadType(nullptr),
		m_hWndBtnPassShowOrHide(nullptr),
		m_hWndBtnRomOrNum(nullptr),
		m_hWndBtnKeyReloc(nullptr),
		m_hWndBtnShowOrHidePad(nullptr)
	{

	}

	CreaKeyPad::~CreaKeyPad() {
		// 폰트 핸들이 있으면 해제하여야 한다.
		// 스마트 포인터가 적용된 폰트 핸들을 명시적 해제
		if (m_hFontStDisp) {
			m_hFontStDisp.reset(nullptr);
		}
	}

	bool CreaKeyPad::CreateKeyPad(HWND hWndParent, HINSTANCE hInst, POINT ptStart, SIZE szBtnSize, int iIDS_BTN_FIRST, int iPadding, int iCols, bool bRomanNumber) {
		int i;

		m_bShowPass = true;
		m_bShowRoman = bRomanNumber;
		m_bShowPad = true;
		m_iKeypadFontHeight = 0;
		m_hFontStDisp = nullptr;

		if (iCols < 3 || iCols > 5) {
			m_iCols = 5;
		}
		else {
			m_iCols = iCols;
		}

		m_ptStart = ptStart;
		m_szBtnSize = szBtnSize;

		m_iIDS_BTN_FIRST = iIDS_BTN_FIRST;
		m_bShowRoman = bRomanNumber;

		m_hWndStDispNum = CreateWindow(_T("static"), nullptr, WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 0, 0, hWndParent, (HMENU)iIDS_BTN_FIRST + 13, hInst, nullptr);

		// 버튼 10개 생성 단 버튼의 타이틀은 없음
		for (i = 0; i < 10; i++) {
			if (!m_arObjKeys[i].CreateBtn(_T(""), nullptr, hWndParent, iIDS_BTN_FIRST + i, hInst, 0, iPadding)) {
				break;
			}
		}

		if (i < 10) {
			return false;
		}

		RelocKeypad();

		// 하나 삭제 버튼
		if (!m_arObjKeys[i].CreateBtn(m_lparctCtrlBtnTitles[0], nullptr, hWndParent, iIDS_BTN_FIRST + i, hInst, 0, iPadding)) {
			return false;
		}

		i++;

		// 전체 삭제 버튼(이건 에디트 컨트롤 옆에 두기로 한다.)
		if (!m_arObjKeys[i].CreateBtn(m_lparctCtrlBtnTitles[1], nullptr, hWndParent, iIDS_BTN_FIRST + i, hInst, 0, iPadding)) {
			return false;
		}

		i++;

		// 입력 버튼
		if (!m_arObjKeys[i].CreateBtn(m_lparctCtrlBtnTitles[2], nullptr, hWndParent, iIDS_BTN_FIRST + i, hInst, 0, iPadding)) {
			return false;
		}

		i++;

		// 키 패드 위에 설정 컨트롤 생성
		// DropDown

		if (!m_hWndCmbPadType) {
			m_hWndCmbPadType = CreateWindow(
				WC_COMBOBOX,              // 클래스 이름: 콤보 박스
				nullptr,                     // 초기 텍스트
				CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL, // 스타일
				0, 0,                  // 위치 (x, y)
				0, 0,                 // 크기 (너비, 높이)
				hWndParent,                    // 부모 윈도우 핸들
				(HMENU)(iIDS_BTN_FIRST + i),                // 메뉴 ID
				hInst,
				nullptr);


		}
		else {
			SendMessage(m_hWndCmbPadType, CB_RESETCONTENT, 0, 0);
		}


		// 콤보 박스에 항목 추가
		SendMessage(m_hWndCmbPadType, CB_ADDSTRING, 0, (LPARAM)_T("PadType 3"));
		SendMessage(m_hWndCmbPadType, CB_ADDSTRING, 0, (LPARAM)_T("PadType 4"));
		SendMessage(m_hWndCmbPadType, CB_ADDSTRING, 0, (LPARAM)_T("PadType 5"));

		SendMessage(m_hWndCmbPadType, CB_SETCURSEL, (WPARAM)m_iCols - 3, 0);

		i++;

		// 비번 보이기 또는 숨기기 버튼 생성(표시가 기본)
		if (!m_hWndBtnPassShowOrHide) {
			m_hWndBtnPassShowOrHide = CreateWindow(_T("button"), m_lparctShowHideBtnTitles[1], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 0, 0,
				hWndParent, (HMENU)(iIDS_BTN_FIRST + i), hInst, nullptr);
		}

		i++;
		// 로마자 또는 숫자 키 페드 토글 버튼(생성시 인자값으로 결정)
		if (!m_hWndBtnRomOrNum) {
			m_hWndBtnRomOrNum = CreateWindow(_T("button"), m_lparctRomOrNumBtnTitles[m_bShowRoman ? 1 : 0], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 0, 0,
				hWndParent, (HMENU)(iIDS_BTN_FIRST + i), hInst, nullptr);
		}


		i++;
		if (!m_hWndBtnKeyReloc) {
			m_hWndBtnKeyReloc = CreateWindow(_T("button"), _T("키 재배치"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 0, 0,
				hWndParent, (HMENU)(iIDS_BTN_FIRST + i), hInst, nullptr);
		}

		i++;
		if (!m_hWndBtnShowOrHidePad) {
			m_hWndBtnShowOrHidePad = CreateWindow(_T("button"), m_lparctShowPadBtnTitles[m_bShowPad ? 1 : 0], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 0, 0,
				hWndParent, (HMENU)(iIDS_BTN_FIRST + i), hInst, nullptr);
		}

		KeypadArrangement();

		SetKeypadFontHeight(0);

		return true;
	}

	bool CreaKeyPad::OnDrawItem(LPARAM lParam) {
		int i;

		for (i = 0; i < 13; i++) {
			if (m_arObjKeys[i].OnDrawItem(lParam)) {
				return true;
			}
		}

		return false;
	}

	bool CreaKeyPad::OnCommand(WORD wID, WORD wEvt, LPCTSTR* ppctMsg) {
		bool bRetVal = false;
		int iIDS = wID - m_iIDS_BTN_FIRST;
		tstring strTxt;

		int i;
		bool bIsNum = true;
		int iItemIdx;
		int iLen;

		RECT rtBtnInput;

		// 숫자 키 버튼을 눌렀을 때
		if (iIDS >= 0 && iIDS < 10) {
			if (!m_arObjKeys[iIDS].GetHWndBtn()) {
				return false;
			}

			strTxt = m_arObjKeys[iIDS].GetBtnTxt();

			m_arObjKeys[iIDS].SetBtnTxtPos(0);

			for (i = 0; i < strTxt.size(); i++) {
				if (!isdigit(strTxt[i])) {
					bIsNum = false;
					break;
				}
			}

			if (!bIsNum) {
				for (i = 0; i < 10; i++) {
					if (_tcscmp(strTxt.c_str(), m_lparctRomanNumerals[i]) == 0) {
#ifdef _UNICODE
						strTxt = to_wstring(i);
#else
						strTxt = to_string(i);
#endif
						break;
					}
				}
			}

			m_strInputKey.append(strTxt);

			ShowPass();

			bRetVal = true;
		}
		else if (iIDS == 10) {		// 하나 삭제 처리
			if (!m_arObjKeys[iIDS].GetHWndBtn()) {
				return false;
			}

			if (m_strInputKey.length()) {
				m_strInputKey.pop_back();

				ShowPass();
			}

			m_arObjKeys[iIDS].SetBtnTxtPos(0);

			bRetVal = true;
		}
		else if (iIDS == 11) {		// 전체 삭제 처리
			if (!m_arObjKeys[iIDS].GetHWndBtn()) {
				return false;
			}

			m_strInputKey = _T("");

			m_arObjKeys[iIDS].SetBtnTxtPos(0);

			ShowPass();

			bRetVal = true;
		}
		else if (iIDS == 12) {
			if (!m_arObjKeys[iIDS].GetHWndBtn()) {
				return false;
			}

			if (ppctMsg) {
				*ppctMsg = m_strInputKey.c_str();
			}

			m_arObjKeys[iIDS].SetBtnTxtPos(0);

			bRetVal = true;
		}
		else if (iIDS == 13) {		// 콤보박스
			if (!m_hWndCmbPadType) {
				return false;
			}

			if (wEvt == CBN_SELCHANGE) {
				iItemIdx = SendMessage(m_hWndCmbPadType, CB_GETCURSEL, 0, 0); // 선택된 항목 인덱스 얻기
				if (iItemIdx != CB_ERR) {
					TCHAR sztItemText[256] = { 0, };
					SendMessage(m_hWndCmbPadType, CB_GETLBTEXT, iItemIdx, (LPARAM)sztItemText); // 선택된 항목 텍스트 얻기
					iLen = _tcslen(sztItemText);

					if (iLen - 1 >= 0) {
						m_iCols = sztItemText[iLen - 1] - _T('0');
						KeypadArrangement();
					}
				}
				bRetVal = true;
			}
		}
		else if (iIDS == 14) {		// 비번 표시 또는 숨김 버튼
			if (!m_hWndBtnPassShowOrHide) {
				return false;
			}

			m_bShowPass = !m_bShowPass;

			if (m_bShowPass) {
				SetWindowText(m_hWndBtnPassShowOrHide, m_lparctShowHideBtnTitles[1]);
			}
			else {
				SetWindowText(m_hWndBtnPassShowOrHide, m_lparctShowHideBtnTitles[0]);
			}

			ShowPass();

			bRetVal = true;
		}
		else if (iIDS == 15) {		// 로마자 또는 숫자 토글 버튼
			if (!m_hWndBtnRomOrNum) {
				return false;
			}

			m_bShowRoman = !m_bShowRoman;

			if (m_bShowRoman) {
				SetWindowText(m_hWndBtnRomOrNum, m_lparctRomOrNumBtnTitles[1]);
			}
			else {
				SetWindowText(m_hWndBtnRomOrNum, m_lparctRomOrNumBtnTitles[0]);
			}

			RelocKeypad();

			ShowPass();

			bRetVal = true;
		}
		else if (iIDS == 16) {		// 키 재배치 버튼
			if (!m_hWndBtnKeyReloc) {
				return false;
			}

			RelocKeypad();

			bRetVal = true;
		}
		else if (iIDS == 17) {		// 패드 표시 또는 숨기기 토글 버튼
			if (!m_hWndBtnShowOrHidePad) {
				return false;
			}

			m_bShowPad = !m_bShowPad;

			if (m_bShowPad) {
				SetWindowText(m_hWndBtnShowOrHidePad, m_lparctShowPadBtnTitles[1]);
			}
			else {
				SetWindowText(m_hWndBtnShowOrHidePad, m_lparctShowPadBtnTitles[0]);
			}

			KeypadArrangement();

			bRetVal = true;
		}

		return bRetVal;
	}

	void CreaKeyPad::KeypadArrangement() {
		RECT rtBtn;
		int i;
		const int iTitleHeightSize = 30;

		UINT uiSWP_Style = 0;

		if (m_bShowPad) {
			uiSWP_Style = SWP_SHOWWINDOW | SWP_NOZORDER;
		}
		else {
			uiSWP_Style = SWP_HIDEWINDOW;
		}

		// 비번을 표시하는 에디트 컨트롤 위치 지정
		SetMyWindowPos(m_hWndStDispNum, HWND_TOP, m_ptStart.x, m_ptStart.y + iTitleHeightSize, m_szBtnSize.cx * m_iCols, m_szBtnSize.cy, uiSWP_Style);
		

		SetRect(&rtBtn, m_ptStart.x, m_ptStart.y + m_szBtnSize.cy + iTitleHeightSize, m_ptStart.x + m_szBtnSize.cx, m_ptStart.y + m_szBtnSize.cy * 2 + iTitleHeightSize);

		// 숫자 버튼 위치 지정
		for (i = 0; i < 10; i++) {
			SetMyWindowPos(m_arObjKeys[i].GetHWndBtn(), HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, uiSWP_Style);

			rtBtn.left += m_szBtnSize.cx;
			rtBtn.right += m_szBtnSize.cx;

			if ((i + 1) % m_iCols == 0) {
				rtBtn.left = m_ptStart.x;
				rtBtn.right = rtBtn.left + m_szBtnSize.cx;

				rtBtn.top += m_szBtnSize.cy;
				rtBtn.bottom += m_szBtnSize.cy;
			}
		}

		// 하나 삭제 버튼
		switch (m_iCols) {
		case 3:
		case 4:
			rtBtn.right += m_szBtnSize.cx;
			break;
		case 5:
			rtBtn.left = m_ptStart.x;
			rtBtn.right = rtBtn.left + m_szBtnSize.cx * 2;
			break;
		default:
			break;
		}

		SetMyWindowPos(m_arObjKeys[i].GetHWndBtn(), HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, uiSWP_Style);


		i++;

		// 입력 버튼
		switch (m_iCols) {
		case 3:
			rtBtn.left = m_ptStart.x + m_szBtnSize.cx * 3;
			rtBtn.right = rtBtn.left + m_szBtnSize.cx;

			rtBtn.top = m_ptStart.y + m_szBtnSize.cy + iTitleHeightSize;
			rtBtn.bottom = rtBtn.top + m_szBtnSize.cy * 4;
			break;
		case 4:
			rtBtn.left = m_ptStart.x + m_szBtnSize.cx * 4;
			rtBtn.right = rtBtn.left + m_szBtnSize.cx;

			rtBtn.top = m_ptStart.y + m_szBtnSize.cy + iTitleHeightSize;
			rtBtn.bottom = rtBtn.top + m_szBtnSize.cy * 3;
			break;
		case 5:
			rtBtn.left = m_ptStart.x + m_szBtnSize.cx * 2;
			rtBtn.right = rtBtn.left + m_szBtnSize.cx * 3;
			break;
		default:
			break;
		}

		SetMyWindowPos(m_arObjKeys[i + 1].GetHWndBtn(), HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, uiSWP_Style);


		// 키 페드의 부모의 사이즈 조절(키 페드 표시시)
		if (m_bShowPad) {
			SetMyWindowPos(GetParent(m_arObjKeys[i + 1].GetHWndBtn()), HWND_TOP, 0, 0, rtBtn.right + m_ptStart.x + (m_iCols != 5 ? 0 : m_szBtnSize.cx), rtBtn.bottom + m_ptStart.y + iTitleHeightSize, SWP_NOMOVE);
		}

		// 전체 삭제 버튼(이건 에디트 컨트롤 옆에 두기로 한다.)
		rtBtn.left = m_ptStart.x + m_szBtnSize.cx * m_iCols;

		rtBtn.right = rtBtn.left + m_szBtnSize.cx;
		rtBtn.top = m_ptStart.y + iTitleHeightSize;

		rtBtn.bottom = rtBtn.top + m_szBtnSize.cy * (m_iCols != 5 ? 1 : 4);

		SetMyWindowPos(m_arObjKeys[i].GetHWndBtn(), HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, uiSWP_Style);


		// 키 페드의 부모의 사이즈 조절(키 페드 숨길때)
		if (!m_bShowPad) {
			SetMyWindowPos(GetParent(m_arObjKeys[i].GetHWndBtn()), HWND_TOP, 0, 0, rtBtn.right + m_ptStart.x + (m_iCols != 5 ? 0 : m_szBtnSize.cx), rtBtn.top + m_ptStart.y + iTitleHeightSize, SWP_NOMOVE);
		}

		rtBtn.left = m_ptStart.x;
		rtBtn.top = m_ptStart.y;
		rtBtn.right = rtBtn.left + 100;
		rtBtn.bottom = rtBtn.top + 80;

		SetMyWindowPos(m_hWndCmbPadType, HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, SWP_NOZORDER);


		rtBtn.left += 110;
		rtBtn.right = rtBtn.left + 80;

		rtBtn.bottom = rtBtn.top + 25;

		SetMyWindowPos(m_hWndBtnPassShowOrHide, HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, SWP_NOZORDER);


		rtBtn.left += 90;
		rtBtn.right = rtBtn.left + 60;

		SetMyWindowPos(m_hWndBtnRomOrNum, HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, SWP_NOZORDER);


		rtBtn.left += 70;
		rtBtn.right = rtBtn.left + 80;

		SetMyWindowPos(m_hWndBtnKeyReloc, HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, SWP_NOZORDER);


		rtBtn.left += 90;
		rtBtn.right = rtBtn.left + 80;

		SetMyWindowPos(m_hWndBtnShowOrHidePad, HWND_TOP, rtBtn.left, rtBtn.top, rtBtn.right - rtBtn.left, rtBtn.bottom - rtBtn.top, SWP_NOZORDER);


		if (GetParent(m_hWndCmbPadType)) {
			InvalidateRect(GetParent(m_hWndCmbPadType), nullptr, true);
		}
	}

	void CreaKeyPad::SetMyWindowPos(HWND hWnd, HWND hWndInsertAfter, int  iX, int  iY, int  iCX, int  iCY, UINT uFlags) {
		if (hWnd) {		// 윈도우 헨들이 있을 때만 Flag 상황의 위치를 적용한다.
			SetWindowPos(hWnd, hWndInsertAfter, iX, iY, iCX, iCY, uFlags);
		}
	}

	void CreaKeyPad::SetKeypadFontHeight(int iFontHeight) {
		int i;
		int iCalcHeight;

		for (i = 0; i < 13; i++) {
			iCalcHeight = (int)((float)iFontHeight * (i == 11 ? 0.7f : 1.0f));

			if (i == 11) {
				m_arObjKeys[i].SetBtnFontForHeight(iCalcHeight);
			}
			else {
				m_arObjKeys[i].SetBtnFontForHeight(iFontHeight);
			}
		}

		m_iKeypadFontHeight = iFontHeight;

		HFONT hFontBtn = nullptr;

		// LOGFONT 구조체 선언 및 초기화
		LOGFONT logFont;

		RECT rtBtnClient;

		if (m_hWndStDispNum == nullptr) {
			return;
		}

		hFontBtn = (HFONT)SendMessage(m_hWndStDispNum, WM_GETFONT, 0, 0);

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

		if (logFont.lfHeight > 0) {
			logFont.lfHeight = m_iKeypadFontHeight;
		}
		else {
			logFont.lfHeight = -m_iKeypadFontHeight;
		}

		m_hFontStDisp.reset(CreateFontIndirect(&logFont));

		SendMessage(m_hWndStDispNum, WM_SETFONT, (WPARAM)m_hFontStDisp.get(), MAKELONG(TRUE, 0));

		InvalidateRect(GetParent(m_arObjKeys[0].GetHWndBtn()), nullptr, true);
	}

	tstring CreaKeyPad::TransRomanNumber() {
		tstring strRomanNumber;

		int i;
		int iIdx;

		for (i = 0; i < m_strInputKey.size(); i++) {
			iIdx = m_strInputKey[i] - _T('0');
			strRomanNumber.append(m_lparctRomanNumerals[iIdx]);

			if (i < m_strInputKey.size() - 1) {
				strRomanNumber.append(_T(", "));
			}
		}

		return strRomanNumber;
	}

	void CreaKeyPad::ShowPass() {
		int i;
		tstring strTxt;

		if (m_bShowPass) {
			if (m_bShowRoman) {
				SetWindowText(m_hWndStDispNum, TransRomanNumber().c_str());
			}
			else {
				SetWindowText(m_hWndStDispNum, m_strInputKey.c_str());
			}
		}
		else {
			strTxt = _T("");

			for (i = 0; i < m_strInputKey.size(); i++) {
				strTxt.append(m_lpctHidePass);
			}

			SetWindowText(m_hWndStDispNum, strTxt.c_str());
		}
	}

	void CreaKeyPad::RelocKeypad() {
		int i;
		int iRndVal;
		int iPushNum;

		int iResult;

		tstring strBtnTitle;

		vector<int> vecNumList;

		for (i = 0; i < 10; i++) {
			vecNumList.push_back(i);
		}

		i = 0;

		// 버튼 생성(0 ~ 9)
		while (vecNumList.size()) {
			iRndVal = rand() % vecNumList.size();

			iPushNum = vecNumList[iRndVal];

			vecNumList.erase(remove(vecNumList.begin(), vecNumList.end(), iPushNum), vecNumList.end());

			strBtnTitle.resize(1024);
			if (m_bShowRoman) {
				iResult = _stprintf_s(&strBtnTitle[0], 1024, _T("%s"), m_lparctRomanNumerals[iPushNum]);
			}
			else {
				iResult = _stprintf_s(&strBtnTitle[0], 1024, _T("%d"), iPushNum);
			}

			strBtnTitle.resize(iResult);

			m_arObjKeys[i].SetBtnTxt(strBtnTitle);

			m_arObjKeys[i].SetBtnTxtPos(0);

			i++;

			if (i > 9) {
				break;
			}
		}
	}
}
