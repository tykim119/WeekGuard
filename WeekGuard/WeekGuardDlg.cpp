
// WeekGuardDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "WeekGuard.h"
#include "WeekGuardDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWeekGuardDlg 대화 상자



CWeekGuardDlg::CWeekGuardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWeekGuardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nTimeLimitSeconds = GetTimeLimitForDay() * 60; // 요일에 따른 기본 시간 제한 설정
	m_nRemainingSeconds = LoadRemainingTime();
}

void CWeekGuardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWeekGuardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, &CWeekGuardDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWeekGuardDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWeekGuardDlg 메시지 처리기

BOOL CWeekGuardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	/*
	UpdateData(TRUE);
	std::thread timerThread([this]() { this->StartUsageTimer(); });
	timerThread.detach();
	//
	SetTimer(0, 100, NULL);
	*/

	CRect screenRect;
	GetDesktopWindow()->GetWindowRect(&screenRect);
	MoveWindow(screenRect);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CWeekGuardDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CWeekGuardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 배경색을 노란색으로 채우고 텍스트를 그리기 위한 작업
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		CRect rect;
		GetClientRect(&rect);

		// 배경색을 노란색으로 채움
		CBrush brush(RGB(0, 0, 0));  // 노란색 배경
		dc.FillRect(&rect, &brush);

		// 텍스트 설정
		CString strText = _T("v1.0.0.0 (대율아빠)");

		// 텍스트 그리기 위치 계산 (우측 하단)
		rect.left = rect.left + 10;  // 텍스트의 최대 너비를 200으로 설정
		rect.top = rect.bottom - 50;   // 텍스트의 최대 높이를 50으로 설정

		// 텍스트 색상을 노란색으로 설정
		dc.SetTextColor(RGB(255, 255, 0));  // 텍스트는 검정색으로 설정
		dc.SetBkMode(TRANSPARENT);  // 배경을 투명하게 설정
		//dc.SetTextAlign(TA_RIGHT | TA_BOTTOM);  // 우측 하단 정렬
		dc.DrawText(strText, &rect, DT_SINGLELINE | DT_VCENTER);


	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CWeekGuardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CWeekGuardDlg::GetTimeLimitForDay() {
	std::time_t t = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &t);

	// 평일 (월요일~금요일)은 30분, 주말 (토요일, 일요일)은 120분 설정
	if (localTime.tm_wday == 0 || localTime.tm_wday == 6) {
		return 11; // 주말
	}
	else {
		return 30;  // 평일
	}
}

int CWeekGuardDlg::LoadRemainingTime() {
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	std::wstring usageTimeFilePath = std::wstring(szPath) + L"\\usetime.ini";

	std::ifstream inFile(usageTimeFilePath);
	int remainingTime = m_nTimeLimitSeconds;
	std::time_t lastSavedTime = 0;

	if (inFile.is_open()) {
		inFile >> remainingTime;
		inFile >> lastSavedTime;  // 마지막 저장된 날짜 읽기
		inFile.close();
	}

	// 현재 날짜
	std::time_t currentTime = std::time(nullptr);
	std::tm currentDate;
	localtime_s(&currentDate, &currentTime);

	// 마지막 저장된 날짜
	std::tm savedDate;
	localtime_s(&savedDate, &lastSavedTime);

	if (currentDate.tm_yday != savedDate.tm_yday || currentDate.tm_year != savedDate.tm_year) {
		// 날짜가 다르면 시간을 초기화
		remainingTime = m_nTimeLimitSeconds;
	}

	return (remainingTime > 0) ? remainingTime : m_nTimeLimitSeconds;
}

void CWeekGuardDlg::SaveRemainingTime(int remainingTime) {
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	std::wstring usageTimeFilePath = std::wstring(szPath) + L"\\usetime.ini";

	std::ofstream outFile(usageTimeFilePath);
	if (outFile.is_open()) {
		outFile << remainingTime << std::endl;
		std::time_t currentTime = std::time(nullptr);
		outFile << currentTime << std::endl;  // 현재 날짜 저장
		outFile.close();
	}
}

void CWeekGuardDlg::ShowNonBlockingMessage(const CString& message) {
	CWnd* pWnd = new CWnd();
	if (pWnd->CreateEx(0, AfxRegisterWndClass(0), message, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 100, nullptr, nullptr)) {
		pWnd->ShowWindow(SW_SHOWNORMAL);
		std::thread([pWnd]() {
			std::this_thread::sleep_for(std::chrono::seconds(10));
			pWnd->PostMessage(WM_CLOSE);
		}).detach();
	}
	else {
		delete pWnd;
	}
}

void CWeekGuardDlg::StartUsageTimer() {
	while (m_nRemainingSeconds > 0) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_nRemainingSeconds--;
		SaveRemainingTime(m_nRemainingSeconds);
		/*
		if (m_nRemainingSeconds == 600) { // 600초 == 10분
			CString warningMessage;
			warningMessage.Format(_T("경고: 10분 남았습니다!"));
			ShowNonBlockingMessage(warningMessage);
		}

		if (m_nRemainingSeconds <= 0) {
			ShowNonBlockingMessage(_T("사용 시간이 종료되었습니다. 컴퓨터가 종료됩니다."));
			std::this_thread::sleep_for(std::chrono::seconds(5)); // 메시지를 잠시 표시한 후 종료
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		*/
		if (m_nRemainingSeconds <= 0) {
			std::this_thread::sleep_for(std::chrono::seconds(5)); // 메시지를 잠시 표시한 후 종료
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}

	}
}

void CWeekGuardDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(nIDEvent);
	ShowWindow(SW_HIDE);

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CWeekGuardDlg::OnEraseBkgnd(CDC* pDC)
{
#if 1
	CRect rect;
	GetClientRect(&rect);

	// 배경색을 원하는 색으로 채움 (예: 검정색)
	CBrush brush(RGB(0, 0, 0));
	pDC->FillRect(&rect, &brush);
	return TRUE;  // 기본 동작을 막음
#else
	return CDialogEx::OnEraseBkgnd(pDC);
#endif
}


void CWeekGuardDlg::OnBnClickedButton1()
{
	CString strBuffer;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strBuffer);
	if (strBuffer == L"dkanrjsk1!") {
		// 화면 숨기기
		ShowWindow(SW_HIDE);
	}
	else {
		AfxMessageBox(L"패스워드가 트렸지롱~~");
	}
}


void CWeekGuardDlg::OnBnClickedButton2()
{
	// 화면 숨기기
	ShowWindow(SW_HIDE);

	m_pCTimeDlg = new CTimeDlg();
	m_pCTimeDlg->Create(IDD_DIALOG_TIME, this);
	m_pCTimeDlg->ShowWindow(SW_SHOW);

	// 시간 타이머
	std::thread timerThread([this]() { this->StartUsageTimer(); });
	timerThread.detach();

}
