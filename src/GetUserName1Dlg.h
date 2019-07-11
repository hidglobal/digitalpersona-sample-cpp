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

// GetUserName1Dlg.h

#pragma once

class CGetUserName1Dlg : public CDialogImpl<CGetUserName1Dlg>, public CUserNameHelper  {
public:
	enum { IDD = IDD_USER_NAME1 };

	BEGIN_MSG_MAP(CGetUserName1Dlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDOK, OnOK)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam,  BOOL& bHandled) {
		Init(GetDlgItem(IDC_COMBO_USERTYPES));
		return 1;
	}

	LRESULT OnCancel(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		EndDialog(IDCANCEL);
		return 0;
	}

	LRESULT OnOK(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		GetName(GetDlgItem(IDC_EDIT_USER_NAME), GetDlgItem(IDC_COMBO_USERTYPES));
		if (!CheckName(m_sUserName, m_uUserNameType)) {
			MessageBox(L"Name does not correspond to the selected format", L"DigitalPersona SDK Sample", MB_OK);
			return 0;
		}
		GetDlgItemText(IDC_EDIT_USER_NAME,   m_sUserName);
		EndDialog(IDOK);
		return 0;
	}
};



