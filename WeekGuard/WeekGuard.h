
// WeekGuard.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CWeekGuardApp:
// �� Ŭ������ ������ ���ؼ��� WeekGuard.cpp�� �����Ͻʽÿ�.
//

class CWeekGuardApp : public CWinApp
{
public:
	CWeekGuardApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CWeekGuardApp theApp;