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

// UserNameHelper.h
#pragma once

class CUserNameHelper {
public:
	LPCWSTR             UserName()     const { return m_sUserName;     }
	DPAl_USERNAME_TYPES UserNameType() const { return m_uUserNameType; }

protected: 
	CUserNameHelper() : m_uUserNameType(DPAl_USER_NAME_DP_ACCOUNT) {}

	static bool CheckName(LPCWSTR szUserName, ULONG uUserNameType) {
		switch(uUserNameType) {
			case DPAl_USER_NAME_SAM_COMPATIBLE:
				if (wcschr(szUserName, '\\') != NULL) return true;
				break;
			case DPAl_USER_NAME_USER_PRINCIPAL:
				if (wcschr(szUserName, '@') != NULL) return true;
				break;
			case DPAl_USER_NAME_DP_ACCOUNT:
				return true;
		}
		return false;
	}

	void Init(HWND hComboBox) {
		int iItem;

		iItem = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"DPAl_USER_NAME_DP_ACCOUNT");
		SendMessage(hComboBox, CB_SETITEMDATA, iItem, DPAl_USER_NAME_DP_ACCOUNT);

		iItem = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"DPAl_USER_NAME_SAM_COMPATIBLE");
		SendMessage(hComboBox, CB_SETITEMDATA, iItem, DPAl_USER_NAME_SAM_COMPATIBLE);

		iItem = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"DPAl_USER_NAME_USER_PRINCIPAL");
		SendMessage(hComboBox, CB_SETITEMDATA, iItem, DPAl_USER_NAME_USER_PRINCIPAL);

		iItem = SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	}

	void GetName(HWND hEdiControl, HWND hComboBox) {
		WCHAR szUserName[MAX_PATH+1] = {0};
		GetWindowText(hEdiControl, szUserName, MAX_PATH);
		m_sUserName = szUserName;
		int iSel = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
		m_uUserNameType = (DPAl_USERNAME_TYPES)SendMessage(hComboBox, CB_GETITEMDATA, iSel, 0);
	}

	CString              m_sUserName;
	DPAl_USERNAME_TYPES  m_uUserNameType;
};
