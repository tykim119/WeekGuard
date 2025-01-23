
// WeekGuardDlg.h : ��� ����
//

#pragma once

#include "TimeDlg.h"
// CWeekGuardDlg ��ȭ ����
class CWeekGuardDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CWeekGuardDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	int m_nRemainingSeconds;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WEEKGUARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	int GetTimeLimitForDay();
	int LoadRemainingTime();
	void SaveRemainingTime(int remainingTime);
	void ShowNonBlockingMessage(const CString& message);
	void StartUsageTimer();
	void ShowBlackScreen();

	int m_nTimeLimitSeconds;

	CTimeDlg	*m_pCTimeDlg;
// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
