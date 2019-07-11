//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2019 DigitalPersona, Inc.
//
// DigitalPersona Authentication SDK
//
//////////////////////////////////////////////////////////////////////////////
//
// Sample code
//
// This source code is only intended to demonstrate the usage of the
// DigitalPersona Pro Authentication SDK for a few standard cases.  
// For more detailed information regarding the SDK, consult the documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UsingDigitalPersonaSDK.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	::CoInitialize(0);
	DPAlInit();

	CMainDlg dlg;
	dlg.DoModal();

	DPAlTerm();
	::CoUninitialize();

	return 0;
}
