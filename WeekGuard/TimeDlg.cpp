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
	// Ÿ�̸� ���� (1�ʸ��� ������Ʈ)
	SetTimer(1, 1000, nullptr);

	// ���� ��� �귯�� ����
	m_bgBrush.CreateSolidBrush(RGB(0, 0, 0));

	// ���̾�α׸� Topmost�� ����
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	// ��ü ȭ�� ũ�� ��������
	RECT screenRect;
	::GetWindowRect(::GetDesktopWindow(), &screenRect);  // ��ü ȭ�� ũ��

	// �۾� ���� ũ�� �������� (�۾� ǥ���� ���� ����)
	RECT workArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);  // �۾� ǥ���� ���� ����

	// ���̾�α� ũ�� ���
	CRect dlgRect;
	GetWindowRect(&dlgRect);

	// ���̾�α��� ��ġ�� �۾� ǥ���� �ٷ� ���� ����
	int xPos = screenRect.right - dlgRect.Width();  // ȭ�� ���� ��
	int yPos = workArea.bottom - dlgRect.Height();   // �۾� ǥ���� �ٷ� ��

	SetWindowPos(nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);


	// ��Ʈ ���� (���̾�α� ũ�⿡ ���� ũ��)
	CRect rect;
	GetClientRect(&rect);
	int fontSize = rect.Height() / 2;  // ���̾�α� ������ ���� ũ��� ����
	m_timeFont.CreatePointFont(fontSize * 10, _T("���� ���"));  // ��Ʈ ����
	m_timeDisplay.SetFont(&m_timeFont);  // ��Ʈ ����

	UpdateTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CTimeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
#if 1
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIME)
	{
		pDC->SetTextColor(RGB(0, 255, 0));  // ��� �ؽ�Ʈ
		pDC->SetBkColor(RGB(0, 0, 0));      // ���� ���
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
	int hours = nRemainingSeconds / 3600;           // 1�ð��� 3600��
	int minutes = (nRemainingSeconds % 3600) / 60;  // ���������� 1���� 60��
	int seconds = nRemainingSeconds % 60;          // ������ ��

	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d"), hours, minutes, seconds);
	m_timeDisplay.SetWindowText(strTime);
}