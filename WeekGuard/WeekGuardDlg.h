
// WeekGuardDlg.h : 헤더 파일
//

#pragma once


// CWeekGuardDlg 대화 상자
class CWeekGuardDlg : public CDialogEx
{
// 생성입니다.
public:
	CWeekGuardDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WEEKGUARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	int GetTimeLimitForDay();
	int LoadRemainingTime();
	void SaveRemainingTime(int remainingTime);
	void ShowNonBlockingMessage(const CString& message);
	void StartUsageTimer();
	void ShowBlackScreen();

	int m_nTimeLimitSeconds;
	int m_nRemainingSeconds;
// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
