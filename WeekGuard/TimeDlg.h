#pragma once
#include "afxwin.h"


// CTimeDlg dialog

class CTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeDlg)

public:
	CTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTimeDlg();

	class
		CWeekGuardDlg *m_pCWeekGuardDlg;

private:
	void UpdateTime();
	CBrush m_bgBrush;
	CFont m_timeFont;

// Dialog Data
	enum { IDD = IDD_DIALOG_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_timeDisplay;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
