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
// DigitalPersona Authentication SDK for a few standard cases.  
// For more detailed information regarding the SDK, consult the documentation.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include <DPAuthSdkApi.h>

#include "GetSecretNameDlg.h"
#include "SecretNameContentDlg.h"
#include "GetUserAndSecretNamesDlg.h"
#include "GetUserNameDlg.h"
#include "GetUserName1Dlg.h"

class CMainDlg : public CDialogImpl<CMainDlg> {
public:
	enum { IDD = IDD_MAIN };

	BEGIN_MSG_MAP(CMainDlg)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

		// Authentication samples
		COMMAND_ID_HANDLER(IDC_BTN_AUTH1, SimpleAuthenticateCurrentlyLoggedOnUser)
		COMMAND_ID_HANDLER(IDC_BTN_AUTH2, AuthenticateUserProvidingParameters)
		COMMAND_ID_HANDLER(IDC_BTN_AUTH3, GetSecretOfCurrentlyLoggedOnUser)

		// Identification samples
		COMMAND_ID_HANDLER(IDC_BTN_IDENT1, IdentifyUserUsingCurrentlyLoggedOnUserAsDefault)
		COMMAND_ID_HANDLER(IDC_BTN_IDENT2, IdentifyUserProvidingDefaultUserName)
		COMMAND_ID_HANDLER(IDC_BTN_IDENT3, IdentifyUserAndGetUsersSecret)

		// Working with Secrets samples
		COMMAND_ID_HANDLER(IDC_BTN_SECRET1, WriteSecretForCurrentlyLoggedOnUser)
		COMMAND_ID_HANDLER(IDC_BTN_SECRET2, DoesSecretExist)
		COMMAND_ID_HANDLER(IDC_BTN_SECRET3, DeleteSecretForCurrentlyLoggedOnUser)

		// Custom Authentication Policies (Advanced Users Only)
		COMMAND_ID_HANDLER(IDC_BTN_POLICY1, ShowUserAuthenticationPolicy)
		COMMAND_ID_HANDLER(IDC_BTN_AUTH4, AuthenticateCurrentlyLoggedOnUserWithApplicationProvidedAuthenticationPolicy)
		COMMAND_ID_HANDLER(IDC_BTN_AUTH5, AuthenticateCurrentlyLoggedOnUserWithAuthenticationPolicyModifiedByApplication)
		COMMAND_ID_HANDLER(IDC_BTN_IDENT4, IdentifyUserWithCustomAuthenticationPolicy)
	END_MSG_MAP()

	LRESULT OnCancel(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		EndDialog(IDCANCEL);
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Authentication samples                                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates the simplest possible call to authenticate the currently logged on user.
	// The authentication is done with the authentication policy configured by the DigitalPersona administrator.
	// No secrets are released, no strings are customized in the dialog and no mask of verified credentials 
	//is returned.
	LRESULT SimpleAuthenticateCurrentlyLoggedOnUser(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		HRESULT hr = DPAlAuthenticate(m_hWnd, NULL, NULL, NULL, DPAl_USER_NAME_UNKNOWN, NULL, 0, NULL, NULL, NULL, NULL);
		CString sResult;
		if (SUCCEEDED(hr))
			sResult = L"Authentication succeeded.";
		else
			sResult = MyFormatMessage(hr); // In case of an error, return text of the error message for display.

		SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates user authentication.  We provide the username, specify the caption and 
	// text in the authentication dialog and receive the mask of authenticated credentials.
	// The standard authentication policy configured by the Pro administrator is used, and no user secret is released.
	LRESULT AuthenticateUserProvidingParameters(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		CGetUserName1Dlg dlg;
		if (IDOK == dlg.DoModal()) { // Show dialog to get name of the secret to be released (the secret must have been saved before)
			UINT64 uAuthenticatedCredentials = 0i64;
			HRESULT hr = DPAlAuthenticate(m_hWnd,                          // Handle of the parent window (this dialog)
			                               L"DigitalPersona SDK Sample",   // Caption for the authentication dialog
			                               L"Authenticating user providing some parameters.",  // Prompt text for the authentication dialog
			                               dlg.UserName(),                 // User name to authenticate. Note that for non-Kiosk environments it MUST be the currently logged on user or NULL.
										   dlg.UserNameType(),
			                               NULL,                           // Using standard authentication policy
			                               0,                              // Using standard authentication policy
			                               NULL,                           // No secret release needed
			                               NULL,                           // No secret release needed 
			                               NULL,                           // No secret release needed
			                               &uAuthenticatedCredentials);    // Receives mask of credentials used for authentication
			CString sResult;
			if (SUCCEEDED(hr))
				sResult.Format(L"Authentication succeeded.\r\nCredentials used: 0x%016I64X", uAuthenticatedCredentials);
			else
				sResult = MyFormatMessage(hr); // In case of an error, return text of the error message for display.

			SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");

		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates authentication of the currently logged on user.
	// We specify the caption and text for the authentication dialog and receive back the mask of 
	// verified credentials. 
	// The standard authentication policy, configured by the DigitalPersona administrator, is used.
	// After the user is authenticated, the user secret is released and returned to the application.
	LRESULT GetSecretOfCurrentlyLoggedOnUser(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		CGetSecretNameDlg dlg;
		if (IDOK == dlg.DoModal()) { // Show dialog to get name of the secret to be released (the secret must have been saved before)
			ULONG uSecretLength = 0;
			LPBYTE pSecretBuffer = NULL;
			UINT64 uAuthenticatedCredentials = 0;

			HRESULT hr = DPAlAuthenticate(m_hWnd,                        // Handle of the parent window (this dialog) 
			                               L"DigitalPersona SDK Sample", // Caption for the authentication dialog
			                               L"Authenticate currently logged on user and get a secret for that user after authentication.", 
			                               NULL,                         // Null for currently logged on user name
										   DPAl_USER_NAME_UNKNOWN,
			                               NULL,                         // Use standard authentication policy
			                               0,                            // Use standard authentication policy
			                               dlg.m_sSecretName,            // Name of the secret to receive
			                               &uSecretLength,               // Receives length of the buffer containing the secret 
			                               &pSecretBuffer,               // Receives secret content
			                               &uAuthenticatedCredentials);  // Received mask of credentials used for authentication
			CString sResult;
			if (SUCCEEDED(hr)) {
				sResult.Format(L"Authentication succeeded.\r\nSecret length: %lu\r\nSecret Content: %ls\r\nCredentials used: 0x%016I64X", 
				               uSecretLength, (LPCWSTR)pSecretBuffer, uAuthenticatedCredentials);
				DPAlBufferFree(pSecretBuffer); // Release memory used for the secret BLOB 
			}
			else
				sResult = MyFormatMessage(hr); // In case of an error, return text of the error message for display.

			SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Identification samples                                                                                          //
// (for DigitalPersona Kiosk environments only)                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates the simplest possible call to DPAlIdentAuthenticate.
	// Authentication is done with the authentication policy configured by the DigitalPersona administrator.
	// No secrets are released, no strings are provided to the identification dialog and no mask of verified credentials is returned. 
	// By default, the identification dialog shows the username of the currently logged on user.
	// The name of the identified and authenticated user is returned to the application.
	LRESULT IdentifyUserUsingCurrentlyLoggedOnUserAsDefault(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		LPWSTR szUserName = NULL;
		ULONG uUserNameType = DPAl_USER_NAME_UNKNOWN;
		HRESULT hr = DPAlIdentAuthenticate(m_hWnd, NULL, NULL,  NULL, DPAl_USER_NAME_UNKNOWN, NULL, 0, NULL, NULL, NULL, &szUserName, &uUserNameType, NULL);
		CString sResult;
		if (SUCCEEDED(hr)) {
			sResult.Format(L"Identification and authentication succeeded.\r\nUser: %ls (type=%lu)", szUserName, uUserNameType);
			DPAlBufferFree(szUserName); // Release the memory buffer used to return the user name
		}
		else
			sResult = MyFormatMessage(hr); // In case of an error, return text of the error message for display.

		SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates identification of a user. The name of the default user is provided to the dialog.
	// Identification is done in accordance with the authentication policy configured by the DigitalPersona administrator.
	// No secrets are released, no strings are provided to the identification dialog. 
	// The identification dialog shows the default user name specified. This name does not have to be of an existing user, 
	// it is treated simply as a string and the user can change it.
	// The name of the identified and authenticated user and mask of verified credentials is returned to the application.
	LRESULT IdentifyUserProvidingDefaultUserName(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		CGetUserNameDlg dlg;
		if (IDOK == dlg.DoModal()) {      // Show dialog to get default user name to show in the dialog.
			LPWSTR szUserName = NULL;
			ULONG  uUserNameType = DPAl_USER_NAME_UNKNOWN;
			UINT64 uAuthenticatedCredentials = 0;
			HRESULT hr = DPAlIdentAuthenticate(m_hWnd,                       // Handle of the parent window (this dialog)
			                                    NULL,                         // NULL for default caption text
			                                    NULL,                         // NULL for default prompt text
			                                    dlg.UserName(),              // Default user name to show in the dialog
												dlg.UserNameType(),
			                                    NULL,                         // Use standard authentication policy
			                                    0,                            // Use standard authentication policy
			                                    NULL,                         // No secret release needed
			                                    NULL,                         // No secret release needed 
			                                    NULL,                         // No secret release needed
			                                    &szUserName,                  // Receives name of the identified and authenticated user
												&uUserNameType,
			                                    &uAuthenticatedCredentials);  // Receives mask of credentials used for authentication
			CString sResult;
			if (SUCCEEDED(hr)) {
				sResult.Format(L"Identification and authentication succeeded.\r\nUser: %ls\r\nAithenticated by: 0x%016I64X", szUserName, uAuthenticatedCredentials);
				DPAlBufferFree(szUserName); // Release memory buffer used to return username
			}
			else
				sResult = MyFormatMessage(hr);

			SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates identification of a user, returning the specified user secret. 
	// The identification is done in accordance with the authentication policy configured by the DigitalPersona administrator.
	// No secrets are released, no strings are provided to the identification dialog. 
	// By default, the identification dialog shows the username provided. This name does not have to be of an existing user, 
	// it is treated simply as a string and user can change it.
	// The name of the identified and authenticated user and mask of verified credentials is returned.
	LRESULT IdentifyUserAndGetUsersSecret(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		CGetSecretNameDlg dlg;
		CString sResult;
		if (IDOK == dlg.DoModal()) {
			LPWSTR szUserName = NULL;
			ULONG uUserNameType = DPAl_USER_NAME_UNKNOWN;
			UINT64 uAuthenticatedCredentials = 0;
			ULONG uSecretLength = 0;
			LPBYTE pSecretBuffer = NULL;

			HRESULT hr = DPAlIdentAuthenticate(m_hWnd,                        // Handle of the parent window (this dialog)
			                                    L"DigitalPersona SDK Sample", // Caption for the authentication dialog
			                                    L"Identifying user and getting a secret out", // Prompt text for the authentication dialog
			                                    L"",                          // Empty string for not showing any default name
												DPAl_USER_NAME_UNKNOWN,
			                                    NULL,                         // Use standard authentication policy
			                                    0,                            // Use standard authentication policy
			                                    dlg.m_sSecretName,            // Secret name
			                                    &uSecretLength,               // Receives length of the buffer with secret content
			                                    &pSecretBuffer,               // Receives secret content
			                                    &szUserName,                  // Receives name of the identified and authenticated user
												&uUserNameType,
			                                    &uAuthenticatedCredentials);  // Receives mask of credentials used for authentication
			if (SUCCEEDED(hr)) {
				sResult.Format(L"Identification and authentication succeeded.\r\nUser: %ls\r\nAithenticated by: 0x%016I64X\r\nSecret name: %ls\r\nSecret length: %lu\r\nSecret Content: %ls", 
				               szUserName, uAuthenticatedCredentials, static_cast<LPCWSTR>(dlg.m_sSecretName), uSecretLength, pSecretBuffer);
				DPAlBufferFree(szUserName);    // Release memory used to return the user name 
				DPAlBufferFree(pSecretBuffer); // Release memory used for the secret BLOB 
			}
			else
				sResult = MyFormatMessage(hr);

			SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Working with Secrets samples                                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates writing a secret for the currently logged on user after authenticating the user 
	// according to the current authentication policy. We specify the caption and prompt text for the 
	// authentication dialog and receive the mask of verified credentials.
	//
	// The secret here is created from a string, but in practice it can be any BLOB.
	LRESULT WriteSecretForCurrentlyLoggedOnUser(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		//
		CSecretNameContentDlg dlg;
		if (IDOK == dlg.DoModal()) { // Show dialog to get name of and content of the secret to save in the user record.
			UINT64 uAuthenticatedCredentials = 0;

			HRESULT hr = DPAlWriteSecret(m_hWnd,                                                      // Handle of the parent window (this dialog)
			                              L"DigitalPersona SDK Sample",                               // Caption for the authentication dialog
			                              L"Writing a secret for currently logged on user.",          // Prompt text for the authentication dialog
			                              NULL,                                                       // NULL for user name means use currently logged on user
										  DPAl_USER_NAME_UNKNOWN,
			                              NULL,                                                       // Use standard authentication policy
			                              0,                                                          // Use standard authentication policy
			                              dlg.m_sSecretName,                                          // Name of the secret to write to the user record
			                              (dlg.m_sSecretContent.GetLength()+1) * sizeof(WCHAR),       // Length of the secret BLOB (+1 is to save the terminating NULL of the string, not always necessary)
			                              (const LPBYTE)(static_cast<LPCWSTR>(dlg.m_sSecretContent)), // Secret BLOB - casting from string data
			                              &uAuthenticatedCredentials);                                // Receives mask of credentials used for authentication
			CString sResult;
			if (SUCCEEDED(hr))
				sResult.Format(L"Writing secret succeeded.\r\nCredentials used: 0x%016I64X", uAuthenticatedCredentials);
			else
				sResult = MyFormatMessage(hr);

			SetDlgItemText(IDC_EDIT_RESULT1, sResult); // In case of an error, return text of the error message for display.
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example checks for the existence of a secret in the record of the specified user. 
	// The user must be an DigitalPersona LDS (or DigitalPersona AD) user, otherwise the secret will not be found.
	// The username must be provided in UPN or SAM formats.
	LRESULT DoesSecretExist(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		CGetUserAndSecretNamesDlg dlg;
		if (IDOK == dlg.DoModal()) {      // Show dialog to get names of the user and secret.
			BOOL bExist = DPAlDoesSecretExist(dlg.UserName(),    // Name of the user.
			                                  dlg.UserNameType(),
			                                  dlg.m_sSecretName); // Name of the secret
			CString sResult;
			sResult.Format(L"Secret with name \"%ls\"\r\nfor %ls\r\n***%ls***", 
			               static_cast<LPCWSTR>(dlg.m_sSecretName), wcslen(dlg.UserName()) ? dlg.UserName() : L"currently logged on user", bExist ? L"Exists" : L"Does not exist");

			SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates deleting a secret for the currently logged on user after authenticating the user 
	// according to the current authentication policy. We provide caption and prompt text to the 
	// authentication dialog and receive the mask of verified credentials.
	LRESULT DeleteSecretForCurrentlyLoggedOnUser(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		//
		CGetSecretNameDlg dlg;
		if (IDOK == dlg.DoModal()) { // Show dialog to get name of and content of the secret to save in the user record.
			UINT64 uAuthenticatedCredentials = 0;

			HRESULT hr = DPAlWriteSecret(m_hWnd,                                              // Handle of the parent window (this dialog)
			                              L"DigitalPersona SDK Sample",                       // Caption for the authentication dialog
			                              L"Deleting a secret for currently logged on user.", // Prompt text for the authentication dialog
			                              NULL,                                               // NULL for user name means use currently logged on user
										  DPAl_USER_NAME_UNKNOWN,
			                              NULL,                                               // Use standard authentication policy
			                              0,                                                  // Use standard authentication policy
			                              dlg.m_sSecretName,                                  // Name of the secret to delete from the user record
			                              0,                                                  // 0 for deleting the secret
			                              NULL,                                               // NULL for deleting the secret
			                              &uAuthenticatedCredentials);                        // Receives mask of credentials used for authentication
			CString sResult;
			if (S_OK == hr)
				sResult.Format(L"Deleting secret succeeded.\r\nCredentials used: 0x%016I64X", uAuthenticatedCredentials);
			else if (S_FALSE == hr)
				sResult.Format(L"Secret does not exist.");
			else
				sResult = MyFormatMessage(hr);

			SetDlgItemText(IDC_EDIT_RESULT1, sResult); // In case of an error, return text of the error message for display.
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Custom Authentication Policies (Advanced Users Only)                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// See the Developer Guide for more information on custom authentication policies.
//

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example displays the current authentication policy configured by an administrator for the
	// currently logged on user. User name can be NULL for the currently logged on user, but if 
	// provided, the user name must be in UPN or SAM formats.
	LRESULT ShowUserAuthenticationPolicy(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		UINT64* pPolicyArray = NULL;
		ULONG nNumPolicyArrayItems = 0;

		HRESULT hr = DPAlReadAuthPolicy(NULL,                    // NULL for user name means use currently logged on user
		                                DPAl_USER_NAME_UNKNOWN,
		                                &pPolicyArray,           // Receives policy array (must be deallocated by DPAlBufferFree)
		                                &nNumPolicyArrayItems);  // Receives size of the policy array
		CString sResult;
		if (SUCCEEDED(hr)) {
			sResult = L"Authentication Policy:\r\n";
			for (ULONG i=0; i<nNumPolicyArrayItems; ++i) {
				CString s;
				s.Format(L"0x%016I64X\r\n", pPolicyArray[i]);
				sResult += s;
			}

			DPAlBufferFree(pPolicyArray); // Release memory used for the policy array 
		}
		else {
			sResult.Format(L"DPAlReadAuthPolicy failed:\r\n%ls", static_cast<LPCWSTR>(MyFormatMessage(hr))); // In case of an error, return text of the error message for display.
		}

		SetDlgItemText(IDC_EDIT_RESULT1, sResult);

		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates authentication of the currently logged on user.
	// We provide the caption (but not the prompt text) for the authentication dialog and receive the mask of 
	// verified credentials. We also provide the authentication policy.
	// No secret is released to the application.

	// NOTE: It is not a good idea to attempt releasing a user secret when using a custom authentication policy, because 
	// for secret release, the user must be verified with credentials contained in the authentication policy, 
	// configured by the DigitalPersona administrator, and it is not guaranteed that these credentials exist in the custom policy.
	LRESULT AuthenticateCurrentlyLoggedOnUserWithApplicationProvidedAuthenticationPolicy(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		// Create custom policy: (Password AND Fingerprint) OR (Contactless card AND Fingerprint) OR (PIN AND Bluetooth) OR Smart card
		UINT64 pPolicyArray[] = {
			DPAl_CREDENTIAL_TYPE_PASSWORD    | DPAl_CREDENTIAL_TYPE_FINGERPRINT,
			DPAl_CREDENTIAL_TYPE_CONTACTLESS | DPAl_CREDENTIAL_TYPE_FINGERPRINT,
			DPAl_CREDENTIAL_TYPE_PIN         | DPAl_CREDENTIAL_TYPE_BLUETOOTH,
			DPAl_CREDENTIAL_TYPE_PROXIMITY   | DPAl_CREDENTIAL_TYPE_FINGERPRINT,
			DPAl_CREDENTIAL_TYPE_SMARTCARD,
		};

		UINT64 uAuthenticatedCredentials = 0;

		HRESULT hr = DPAlAuthenticate(m_hWnd,                       // Handle of the parent window (this dialog)
		                              L"DigitalPersona SDK Sample", // Caption for the authentication dialog
		                              NULL,                         // NULL for default prompt text
		                              NULL,                         // Null for currently logged on user name
									  DPAl_USER_NAME_UNKNOWN,
		                              pPolicyArray,                 // Application-provided policy array
		                              ARRAYSIZE(pPolicyArray),      // Length of application-provided policy array
		                              NULL,                         // No secret release needed
		                              NULL,                         // No secret release needed 
		                              NULL,                         // No secret release needed
		                              &uAuthenticatedCredentials);  // Received mask of credentials used for authentication
		CString sResult;
		if (SUCCEEDED(hr))
			sResult.Format(L"Authentication succeeded.\r\nCredentials used: 0x%016I64X", uAuthenticatedCredentials);
		else
			sResult = MyFormatMessage(hr); // In case of an error, return text of the error message for display.

		SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates authentication of the currently logged on user.
	// First, we retrieve the current authentication policy and add Bluetooth credential to every element of the policy 
	// array, effectively making Bluetooth a "mandatory" credential.
	// Then we call funtion to authenticate the user, specifying the prompt text (but not the caption) for the authentication 
	// dialog and receive the mask of verified  credentials. 
	// The secret is released to the application, because this policy authenticates all the credentials required 
	// for secret release, and, in addition (if it was not there originally), Bluetooth.
	LRESULT AuthenticateCurrentlyLoggedOnUserWithAuthenticationPolicyModifiedByApplication(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		CGetSecretNameDlg dlg;
		if (IDOK == dlg.DoModal()) {      // Show dialog to get name of the secret to be released (the secret must have been saved before)
			UINT64* pPolicyArray = NULL;
			ULONG nNumPolicyArrayItems = 0;

			HRESULT hr = DPAlReadAuthPolicy(NULL,                    // NULL for user name means use currently logged on user
			                                DPAl_USER_NAME_UNKNOWN,
			                                &pPolicyArray,           // Receives policy array (must be deallocated by DPAlBufferFree)
			                                &nNumPolicyArrayItems);  // Receives size of the policy array
			if (FAILED(hr)) {
				CString sResult;
				sResult.Format(L"DPAlReadAuthPolicy failed:\r\n%ls", static_cast<LPCWSTR>(MyFormatMessage(hr))); // In case of an error, return text of the error message for display.
				SetDlgItemText(IDC_EDIT_RESULT1, sResult);
				return 0;
			}

			// Modify the policy - add Bluetooth token to every element of the array
			for (ULONG i=0; i<nNumPolicyArrayItems; ++i) 
				pPolicyArray[i] |= DPAl_CREDENTIAL_TYPE_BLUETOOTH;

			ULONG uSecretLength = 0;
			LPBYTE pSecretBuffer = NULL;
			UINT64 uAuthenticatedCredentials = 0;

			hr = DPAlAuthenticate(m_hWnd,                       // Handle of the parent window (this dialog) 
	                              NULL,                         // Null for default caption
	                              L"Authenticate for release of the secret", // Prompt text
	                              NULL,                         // Null for currently logged on user 
								  DPAl_USER_NAME_UNKNOWN,
	                              pPolicyArray,                 // Application-provided policy array
	                              nNumPolicyArrayItems,         // Length of application-provided policy array
	                              dlg.m_sSecretName,            // Name of the secret to receive
	                              &uSecretLength,               // Receives length of the buffer containing the secret 
	                              &pSecretBuffer,               // Receives secret content
	                              &uAuthenticatedCredentials);  // Receives mask of credentials used for authentication

			DPAlBufferFree(pPolicyArray); // Release memory used for the policy array 

			CString sResult;
			if (SUCCEEDED(hr))
				sResult.Format(L"Authentication succeeded.\r\nSecret length: %lu\r\nSecret content: %ls\r\nCredentials used: 0x%016I64X", 
				               uSecretLength, (LPCWSTR)pSecretBuffer, uAuthenticatedCredentials);
			else
				sResult = MyFormatMessage(hr); // In case of an error, return text of the error message for display.

			SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		}
		else
			SetDlgItemText(IDC_EDIT_RESULT1, L"Canceled");
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// This example demonstrates identification of a user and with custom authentication policy. 
	// We specify the prompt text, caption for the authentication dialog and 
	// authentication policy. The authentication policy is actually the administrator-created policy, 
	// to which a Bluetooth token is added as a "mandatory" credential.
	// The identification dialog shows no default user name.
	// The name of the identified and authenticated user and the mask of authenticated credentials are returned to the application.
	//
	// Note that with this approach a user secret may not necessary can be released, because the 
	// modified authentication policy that we provide is based on the policy fop the currently logged on user,
	// and there is no quarantee that for the identified user the policy will be the same. So,
	// we pass NULLs for all secret parameters and do not expect any secret release.
	LRESULT IdentifyUserWithCustomAuthenticationPolicy(WORD, WORD, HWND hWnd, BOOL& bHandled) {
		CString sResult;
		UINT64* pPolicyArray = NULL;
		ULONG nNumPolicyArrayItems = 0;

		HRESULT hr = DPAlReadAuthPolicy(NULL,                      // NULL for user name means use currently logged on user
		                                DPAl_USER_NAME_UNKNOWN,
		                                &pPolicyArray,             // Receives policy array (must be deallocated by DPAlBufferFree)
		                                &nNumPolicyArrayItems);    // Receives size of the policy array
		if (FAILED(hr)) {
			sResult.Format(L"DPAlReadAuthPolicy failed:\r\n%ls", static_cast<LPCWSTR>(MyFormatMessage(hr))); // In case of an error, return text of the error message for display.
			SetDlgItemText(IDC_EDIT_RESULT1, sResult);
			return 0;
		}

		// Modify the policy - add Bluetooth token to every element
		for (ULONG i=0; i<nNumPolicyArrayItems; ++i) 
			pPolicyArray[i] |= DPAl_CREDENTIAL_TYPE_BLUETOOTH;

		LPWSTR szUserName = NULL;
		ULONG uUserNameType = DPAl_USER_NAME_UNKNOWN;
		UINT64 uAuthenticatedCredentials = 0;

		hr = DPAlIdentAuthenticate(m_hWnd,                       // Handle of the parent window (this dialog)
		                           L"DigitalPersona SDK Sample", // Caption for the authentication dialog
		                           L"Identifying user and getting a secret", // Prompt text for the authentication dialog
		                           L"",                          // Empty string for not showing any default name
								   DPAl_USER_NAME_UNKNOWN,
		                           pPolicyArray,                 // Custom policy
		                           nNumPolicyArrayItems,         // Length of the custom policy array
		                           NULL,                         // NULL for Secret name - no secret release
		                           NULL,                         // NULL for address of the length of the buffer for secret content
		                           NULL,                         // NULL for address of the buffer for secret content
		                           &szUserName,                  // Receives name of the identified and authenticated user
								   &uUserNameType,
		                           &uAuthenticatedCredentials);  // Receives mask of credentials used for authentication
		if (SUCCEEDED(hr)) {
			sResult.Format(L"Identification and authentication succeeded.\r\nUser: %ls\r\nAithenticated by: 0x%016I64X", 
			               szUserName, uAuthenticatedCredentials);
			DPAlBufferFree(szUserName);     // Release memory used for the user name 
		}
		else
			sResult = MyFormatMessage(hr);

		DPAlBufferFree(pPolicyArray); // Release memory used for the policy array 

		SetDlgItemText(IDC_EDIT_RESULT1, sResult);
		return 0;
	}


private:
	CString MyFormatMessage(HRESULT hr) {
		LPWSTR szMessageText = NULL;
		DPAlFormatMessage(hr, NULL, &szMessageText);

		CString s = L"UNKNOWN ERROR";
		if (szMessageText) {
			s = szMessageText;
			DPAlBufferFree(szMessageText); // Release memory used for the message text 
		}
		return s;
	}
};

