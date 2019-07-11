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

// GetSecretNameDlg.h
#pragma once

class CGetSecretNameDlg : public CDialogImpl<CGetSecretNameDlg> {
public:
	enum { IDD = IDD_SECRET_NAME };

	BEGIN_MSG_MAP(CGetSecretNameDlg)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDOK, OnOK)
	END_MSG_MAP()


	LRESULT OnCancel(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		EndDialog(IDCANCEL);
		return 0;
	}

	LRESULT OnOK(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		GetDlgItemText(IDC_EDIT_SECRET_NAME, m_sSecretName);
		EndDialog(IDOK);
		return 0;
	}

	CString m_sSecretName;
};
