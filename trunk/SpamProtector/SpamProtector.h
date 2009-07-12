// SpamProtector.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 
#ifdef SMARTPHONE2003_UI_MODEL
#include "resourcesp.h"
#endif

#include "SpamProtectorConfiguration.h"

// CSpamProtectorApp:
// See SpamProtector.cpp for the implementation of this class
//

class CSpamProtectorApp : public CWinApp
{
public:
	CSpamProtectorApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

public:
	CSpamProtectorConfiguration m_Configuration;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSpamProtectorApp theApp;
