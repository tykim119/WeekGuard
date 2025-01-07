// TimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WeekGuard.h"
#include "TimeDlg.h"
#include "afxdialogex.h"
#include "WeekGuardDlg.h"

// CTimeDlg dialog

IMPLEMENT_DYNAMIC(CTimeDlg, CDialogEx)

CTimeDlg::CTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTimeDlg::IDD, pParent)
{

}

CTimeDlg::~CTimeDlg()
{
}

void CTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIME, m_timeDisplay);
}


BEGIN_MESSAGE_MAP(CTimeDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTimeDlg message handlers


BOOL CTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_pCWeekGuardDlg = (CWeekGuardDlg*)AfxGetMainWnd();
	// 타이머 설정 (1초마다 업데이트)
	SetTimer(1, 1000, nullptr);

	// 검정 배경 브러시 생성
	m_bgBrush.CreateSolidBrush(RGB(0, 0, 0));

	// 다이얼로그를 Topmost로 설정
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	// 전체 화면 크기 가져오기
	RECT screenRect;
	::GetWindowRect(::GetDesktopWindow(), &screenRect);  // 전체 화면 크기

	// 작업 영역 크기 가져오기 (작업 표시줄 제외 영역)
	RECT workArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);  // 작업 표시줄 제외 영역

	// 다이얼로그 크기 얻기
	CRect dlgRect;
	GetWindowRect(&dlgRect);

	// 다이얼로그의 위치를 작업 표시줄 바로 위로 설정
	int xPos = screenRect.right - dlgRect.Width();  // 화면 우측 끝
	int yPos = workArea.bottom - dlgRect.Height();   // 작업 표시줄 바로 위

	SetWindowPos(nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);


	// 폰트 설정 (다이얼로그 크기에 맞춰 크게)
	CRect rect;
	GetClientRect(&rect);
	int fontSize = rect.Height() / 2;  // 다이얼로그 높이의 절반 크기로 설정
	m_timeFont.CreatePointFont(fontSize * 10, _T("맑은 고딕"));  // 폰트 생성
	m_timeDisplay.SetFont(&m_timeFont);  // 폰트 적용

	UpdateTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CTimeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
#if 1
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIME)
	{
		pDC->SetTextColor(RGB(0, 255, 0));  // 녹색 텍스트
		pDC->SetBkColor(RGB(0, 0, 0));      // 검정 배경
		return m_bgBrush;
	}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

#else
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
#endif
}


void CTimeDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		UpdateTime();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTimeDlg::UpdateTime()
{
	/*
	CTime currentTime = CTime::GetCurrentTime();
	CString strTime = currentTime.Format(_T("%H:%M:%S"));
	*/
	int nRemainingSeconds = m_pCWeekGuardDlg->m_nRemainingSeconds;
	//
	int hours = nRemainingSeconds / 3600;           // 1시간은 3600초
	int minutes = (nRemainingSeconds % 3600) / 60;  // 나머지에서 1분은 60초
	int seconds = nRemainingSeconds % 60;          // 나머지 초

	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d"), hours, minutes, seconds);
	m_timeDisplay.SetWindowText(strTime);
}