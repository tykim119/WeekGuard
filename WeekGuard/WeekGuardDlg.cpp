
// WeekGuardDlg.cpp : ���� ����
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

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CWeekGuardDlg ��ȭ ����



CWeekGuardDlg::CWeekGuardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWeekGuardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nTimeLimitSeconds = GetTimeLimitForDay() * 60; // ���Ͽ� ���� �⺻ �ð� ���� ����
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


// CWeekGuardDlg �޽��� ó����

BOOL CWeekGuardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

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


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CWeekGuardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// ������ ��������� ä��� �ؽ�Ʈ�� �׸��� ���� �۾�
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		CRect rect;
		GetClientRect(&rect);

		// ������ ��������� ä��
		CBrush brush(RGB(0, 0, 0));  // ����� ���
		dc.FillRect(&rect, &brush);

		// �ؽ�Ʈ ����
		CString strText = _T("v1.0.0.0 (�����ƺ�)");

		// �ؽ�Ʈ �׸��� ��ġ ��� (���� �ϴ�)
		rect.left = rect.left + 10;  // �ؽ�Ʈ�� �ִ� �ʺ� 200���� ����
		rect.top = rect.bottom - 50;   // �ؽ�Ʈ�� �ִ� ���̸� 50���� ����

		// �ؽ�Ʈ ������ ��������� ����
		dc.SetTextColor(RGB(255, 255, 0));  // �ؽ�Ʈ�� ���������� ����
		dc.SetBkMode(TRANSPARENT);  // ����� �����ϰ� ����
		//dc.SetTextAlign(TA_RIGHT | TA_BOTTOM);  // ���� �ϴ� ����
		dc.DrawText(strText, &rect, DT_SINGLELINE | DT_VCENTER);


	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CWeekGuardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CWeekGuardDlg::GetTimeLimitForDay() {
	std::time_t t = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &t);

	// ���� (������~�ݿ���)�� 30��, �ָ� (�����, �Ͽ���)�� 120�� ����
	if (localTime.tm_wday == 0 || localTime.tm_wday == 6) {
		return 11; // �ָ�
	}
	else {
		return 30;  // ����
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
		inFile >> lastSavedTime;  // ������ ����� ��¥ �б�
		inFile.close();
	}

	// ���� ��¥
	std::time_t currentTime = std::time(nullptr);
	std::tm currentDate;
	localtime_s(&currentDate, &currentTime);

	// ������ ����� ��¥
	std::tm savedDate;
	localtime_s(&savedDate, &lastSavedTime);

	if (currentDate.tm_yday != savedDate.tm_yday || currentDate.tm_year != savedDate.tm_year) {
		// ��¥�� �ٸ��� �ð��� �ʱ�ȭ
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
		outFile << currentTime << std::endl;  // ���� ��¥ ����
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
		if (m_nRemainingSeconds == 600) { // 600�� == 10��
			CString warningMessage;
			warningMessage.Format(_T("���: 10�� ���ҽ��ϴ�!"));
			ShowNonBlockingMessage(warningMessage);
		}

		if (m_nRemainingSeconds <= 0) {
			ShowNonBlockingMessage(_T("��� �ð��� ����Ǿ����ϴ�. ��ǻ�Ͱ� ����˴ϴ�."));
			std::this_thread::sleep_for(std::chrono::seconds(5)); // �޽����� ��� ǥ���� �� ����
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		*/
		if (m_nRemainingSeconds <= 0) {
			std::this_thread::sleep_for(std::chrono::seconds(5)); // �޽����� ��� ǥ���� �� ����
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}

	}
}

void CWeekGuardDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	KillTimer(nIDEvent);
	ShowWindow(SW_HIDE);

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CWeekGuardDlg::OnEraseBkgnd(CDC* pDC)
{
#if 1
	CRect rect;
	GetClientRect(&rect);

	// ������ ���ϴ� ������ ä�� (��: ������)
	CBrush brush(RGB(0, 0, 0));
	pDC->FillRect(&rect, &brush);
	return TRUE;  // �⺻ ������ ����
#else
	return CDialogEx::OnEraseBkgnd(pDC);
#endif
}


void CWeekGuardDlg::OnBnClickedButton1()
{
	CString strBuffer;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strBuffer);
	if (strBuffer == L"dkanrjsk1!") {
		// ȭ�� �����
		ShowWindow(SW_HIDE);
	}
	else {
		AfxMessageBox(L"�н����尡 Ʈ������~~");
	}
}


void CWeekGuardDlg::OnBnClickedButton2()
{
	// ȭ�� �����
	ShowWindow(SW_HIDE);

	m_pCTimeDlg = new CTimeDlg();
	m_pCTimeDlg->Create(IDD_DIALOG_TIME, this);
	m_pCTimeDlg->ShowWindow(SW_SHOW);

	// �ð� Ÿ�̸�
	std::thread timerThread([this]() { this->StartUsageTimer(); });
	timerThread.detach();

}
