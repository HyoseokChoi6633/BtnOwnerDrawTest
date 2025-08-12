#pragma once

// 해더 중복을 피하는 해더 선언
#ifndef CMYCREABTNTXTPOSOD
#define CMYCREABTNTXTPOSOD
#include "SSGdi.h"      // 스마트 포인터로 구현된 gdi 객체 선언 헤더
#include <tchar.h>		// 문자열 셋에 따라 변하는 가변 문자 타입
#include <time.h>		// 시간 관련 헤더
#include <string>		// std::basic_string (TCHAR 기반 문자열 타입을 위해)
#include <format>		// 문자열 처리용 헤더
#include <vector>		// 백터 STL 헤더

using namespace std;

// tstring 적용 전처리문
typedef basic_string<TCHAR> tstring;

#endif

class CMyCreaBtnTxtPosOD
{
    // 공적
public:
    CMyCreaBtnTxtPosOD();		// 생성자
    ~CMyCreaBtnTxtPosOD();		// 소멸자

    // 메서드
    bool CreateBtn(LPCTSTR pcszBtnTxt, PRECT prtBtnPos, HWND hWndParent, int iIDS_BTN, HINSTANCE hInst, int iPrtPos, int iPadding);		// 버튼 생성 메서드

    bool OnDrawItem(LPARAM lParam);		// 오너 드로우 메시지 처리 메서드

    HWND GetHWndBtn();					// 버튼의 윈도우 헨들 반환 메서드
    tstring GetBtnTxt();				// 버튼의 텍스트 반환 메서드
    void SetBtnTxt(tstring strBtnTxt);	// 버튼에 표시될 제목 설정 메서드
    void SetBtnTxtPos(int iPrtPos);		// 버튼에 표시될 제목의 위치 설정 메서드

    void SetBtnFontForHeight(int& refiHeight);		// 버튼에 표시될 문자의 높이 설정 메서드(0이면 자동 최적 높이 설정), 버튼의 크기 안에 적절한 높이로 자동 조절

    // 개인
private:
    // 메서드
    void CalcPadding(int iPadding);					// 제 설정할 여백 값을 넣으면 나머지 필요한 멤버 값들을 갱신해 주는 메서드

    // 변수
    HWND m_hWndBtn;			// 버튼 윈도우 헨들
    int m_iIDS_BTN;			// 버튼의 아이디
    int m_iPrtPos;			// 0 이면 출력위치가 렌덤하게 출력
    int m_iPadding;			// 여백 크기

    SIZE m_sizeTxtSize;		// 버튼에 출력될 문자열의 크기(너비, 높이)

    // 스마트 포인터 적용한 폰트 핸들[DeleteObject() 호출을 스마트 포인터로 해결한다.
    UniqueHFont m_hFontBtn;
};
