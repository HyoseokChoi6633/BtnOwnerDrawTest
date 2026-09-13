#pragma once

// OWNER DRAW BUTTON 기술 사용
// 버튼의 제목을 가로3개 세로3개로 9방향 중 랜덤하게 표시하거나(0)
// 1 에서 9 까지 수를 입력하면 명시적으로 그방향에 표시된다.
// 만든 목적은 비밀번호 키 페드의 버튼은 가로 세로 중앙 정렬이라
// 이거 보안상 취약하다고 판단해 만들어 보았다.
// 오너 드로우에 대한 지식만 있다면 쉽게 해석할 것이다.

// 2026-0913_2334 9버튼 스킵 키패드
// 숫자 10 개 중 하나를 무작위로 뺀 키패드를 구현해 본다.
// 9버튼 비번 입력시 스킵된 숫자의 버튼은 누르지 않고 기존 비번 입력을 완성해야 한다.
// 예 3 이 스킵된 버튼의 숫자이면
// 1234 에서 124 를 입력해야 비번 입력이 성공된 것이 된다.

// 해더 중복을 피하는 해더 선언
#ifndef CMYCREAKEYPAD
#define CMYCREAKEYPAD

#ifdef CREAKEYPAD_EXPORTS
#define CREAKEYPAD_API __declspec(dllexport)
#else
#define CREAKEYPAD_API __declspec(dllimport)
#endif

#include "CMyCreaBtnTxtPosOD.h"
#include <commctrl.h>		// 콤보 박스 때문에 선언
#endif

namespace CreaKeyPad_Library
{
	class CREAKEYPAD_API CreaKeyPad
	{
	public:
		// 생성자
		CreaKeyPad();
		// 소멸자
		~CreaKeyPad();

		// 키 페드 생성 메소드
		bool CreateKeyPad(HWND hWndParent, HINSTANCE hInst, POINT ptStart, SIZE szBtnSize, int iIDS_BTN_FIRST, int iPadding, int iCols, bool bRomanNumber = false, int iSkipSpecialNumber = -1);

		// WM_DRAWITEM 시 사용되는 메소드
		bool OnDrawItem(LPARAM lParam);

		// WM_COMMAND 시 사용되는 메소드
		bool OnCommand(WORD wID, WORD wEvt, LPCTSTR* ppctMsg);

		// 페드 키 정렬 메소드
		void KeypadArrangement();

		// 페드 키의 폰트 높이 조절 메소드
		void SetKeypadFontHeight(int iFontHeight);

		// 멤버 상수 선언
		LPCTSTR m_lparctCtrlBtnTitles[3] = { _T("하나 삭제"), _T("전체 삭제"), _T("입력") };		// 키 페드의 입력 관련 컨트롤

		// 키 페드의 상태 관련 버튼 상수
		LPCTSTR m_lparctShowHideBtnTitles[2] = { _T("비번 숨김"), _T("비번 표시") };
		LPCTSTR m_lparctRomOrNumBtnTitles[2] = { _T("숫자"), _T("로마자") };
		LPCTSTR m_lparctShowPadBtnTitles[2] = { _T("패드 숨김"), _T("패드 표시") };
		LPCTSTR m_lpctHidePass = _T("*");

		// 로마자 이용때 사용되는 문자열 배열 상수
		LPCTSTR m_lparctRomanNumerals[10] = {
		_T("NX"),   // 0 (비공식: Nihil)
		_T("I"),    // 1
		_T("II"),   // 2
		_T("III"),  // 3
		_T("IV"),   // 4
		_T("V"),    // 5
		_T("VI"),   // 6
		_T("VII"),  // 7
		_T("VIII"), // 8
		_T("IX")    // 9
		};

	private:
		tstring TransRomanNumber();			// 입력된 비번을 로마자로 변환해 화면에 표시 될때 사용되는 메소드
		void ShowPass();					// 비번을 보이거나 숨길때 사용되는 메소드
		void RelocKeypad();					// 번호 버튼의 제목 숫자를 재 생성시 사용 메소드

		// SetWindowPos 를 정적 메서드로 구현(hWnd 값이 null 이 아닐때만 SetWindowPos() 가 적용 되는 코드 구현)
		void SetMyWindowPos(HWND hWnd, HWND hWndInsertAfter, int  iX, int  iY, int  iCX, int  iCY, UINT uFlags);

		CMyCreaBtnTxtPosOD m_arObjKeys[13];			// 0 ~ 9 까지, 전체지움, 한개지움, 입력 총 13개 버튼 필요 맴버 객체
		POINT m_ptStart;				// 페드의 시작위치 저장 멤버 변수
		SIZE m_szBtnSize;				// 버튼의 기본 크기 저장 멤버 변수
		int m_iCols;					// 페드의 너비 3, 4, 5 중 하나 저장되는 멤버 변수

		HWND m_hWndStDispNum;			// 입력된 비번을 표시할때 사용되는 STATIC 컨트롤
		HWND m_hWndCmbPadType;			// 페드 타입 지정 콤보박스
		HWND m_hWndBtnPassShowOrHide;	// 입력된 비번을 표시할지 아니면 * 처리 해 숨길지 변경하는 버튼
		bool m_bShowPass;				// 위 버튼의 토글 시 사용되는 멤버 변수

		HWND m_hWndBtnRomOrNum;			// 로마자 또는 숫자 표시시 사용되는 버튼
		bool m_bShowRoman;				// 위 버튼의 토글 시 사용되는 멤버 변수

		HWND m_hWndBtnKeyReloc;			// 페드 키의 숫자 부분을 재배치 시켜주는 버튼

		HWND m_hWndBtnShowOrHidePad;	// 페드 키를 보여주거나 숨길때 사용되는 버튼
		bool m_bShowPad;				// 위 버튼의 토글 시 사용되는 멤버 변수

		int m_iIDS_BTN_FIRST;			// 이 페드 컨트롤의 첫번째 윈도우 핸들의 아이디 지정 멤버 변수

		tstring m_strInputKey;			// 입력된 비번이 저장되는 문자열 멤버 변수

		int m_iKeypadFontHeight;		// 키 페드 버튼에서 표시되는 글꼴의 크기 저장

		// 스마트 포인터 적용한 폰트 핸들[DeleteObject() 호출을 스마트 포인터로 해결한다.]
		UniqueHFont m_hFontStDisp;		// 키 페드의 비번표시 STATIC 에서 사용되는 폰트 핸들(글꼴을 변환하기 위해 필요)

		// 9버튼 입력 키패드 입력 시스템
		HWND m_hWndBtnPadTypeSkipSpecialNumber;		// 키 패드 중 특정 숫자 하나를 생략해 버튼을 배치하는 토글 버튼
		int m_iSkipSpecialNumber;				// 위 버튼의 토글 시 사용되는 멤버 변수

		LPCTSTR m_lparctSkipSNPadBtnTitles[2] = { _T("9버튼 패드(%d 미표시)"), _T("10버튼 일반 패드") };

		tstring GetStringBtnSkipNum();			// 10 버튼 또는 9 버튼 조작 버튼의 토글 제목변경 메서드

		HWND m_hWndEditPassNum;					// 자동으로 생성될 숫자 비번을 표시해 주는 Edit 컨트롤

		tstring GenStrPassNum(int iNumDigit);

		HWND m_hWndCmbNumDigit;					// 비밀번호 생성시 자리수 지정 콤보 컨트롤

		HWND m_hWndBtnGenPass;					// 비밀번호를 생성한다.

		tstring GetLastTwoChars(const tstring& str);
		bool SafeStringToInt(const tstring& str, long& outVal);
		tstring GetWindowTextString(HWND hWnd);
		
		// 특정 문자(예: '5')를 모두 제거하는 함수
		tstring RemoveSpecificChar(tstring str, TCHAR targetChar);
	};
}
