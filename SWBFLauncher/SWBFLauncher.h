
// SWBFLauncher.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSWBFLauncherApp:
// See SWBFLauncher.cpp for the implementation of this class
//

class CSWBFLauncherApp : public CWinApp
{
public:
	CSWBFLauncherApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSWBFLauncherApp theApp;
