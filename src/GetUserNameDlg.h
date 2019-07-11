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

// GetUserNameDlg.h
#pragma once


class CGetUserNameDlg : public CDialogImpl<CGetUserNameDlg>, public CUserNameHelper {
public:
	enum { IDD = IDD_USER_NAME };

	BEGIN_MSG_MAP(CGetUserNameDlg)
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
		EndDialog(IDOK);
		return 0;
	}
};



