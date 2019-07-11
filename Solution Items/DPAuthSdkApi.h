/*
Copyright (c) 1996-2012 DigitalPersona, Inc.

MODULE NAME
        DPAuthSdkApi.h

ABSTRACT

DigitalPersona Authentication SDK is a library that allows you to authenticate DigitalPersona users in your applications.  

The SDK leverages the DigitalPersona multi-factor authentication capability:  authentication is performed using 
the DigitalPersona admin's authentication policy and all credential types supported by DigitalPersona (passwords, fingerprints, smart cards, 
etc.) are supported by the API.


OVERVIEW

DigitalPersona Authentication SDK provides C++ functions to authenticate DigitalPersona users and release
their protected data (secrets) after successful authentication.  

The SDK provides:  

1. Authentication:  Verifying that a user is who they claim to be by checking that the 
provided credentials (password, fingerprint, etc.) match their username's credentials in the DigitalPersona database.

The DPAlAuthenticate function displays the multi-factor authentication dialog and matches the supplied
credentials against the user's enrolled credentials.  The customizable dialog box accepts the credentials 
required by the authentication policy set by the Pro admin. 

Optional:  On successful authentication, DPAlAuthenticate can return user secrets to the application.

2. Identification:  Searching in DigitalPersona database to find the user and authenticate them.

The DPAlIdentAuthenticate function displays the multi-factor identification dialog and identifies the user 
based on the credentials supplied. The customizable dialog box allows the user to provide the credentials 
required by the current authentication policy.  

Optional:  If the identification succeeds, DPAlIdentAuthenticate can return the user name and secret to the application. 


Secrets are cryptographically protected and are released to an application only after successful 
authentication of the user. Secrets are stored in the DigitalPersona database and roam with the rest of the user data. 


The most common use case is to add standard DigitalPersona authentication or identification to an application. 

For advanced users, the API provides for custom authentication policies that allow you to:
   - Extend the DigitalPersona admin's policy for more strict authentication (e.g. to add a credentials requirement)
        OR 
   - Create your own authentication policy. In this case, it may not be possible to retrieve secrets if your 
     new policy is not as strict as the DigitalPersona admin's policy

Consult the Developer Guide for more information on custom authentication policies.

*/

#ifndef __DPAl_SDK__
#define __DPAl_SDK__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _WINDOWS_
#pragma error("windows.h must be included before this file.")
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DPAl_EXPORTS
	#ifdef _WIN64
		#define DPAl_STDAPI         EXTERN_C HRESULT STDAPICALLTYPE
		#define DPAl_STDAPI_(type)  EXTERN_C type STDAPICALLTYPE
	#else
		#define DPAl_STDAPI         EXTERN_C __declspec(dllexport) HRESULT STDAPICALLTYPE
		#define DPAl_STDAPI_(type)  EXTERN_C __declspec(dllexport) type STDAPICALLTYPE
	#endif //_WIN64
#else // DPAl_EXPORTS
	#define DPAl_STDAPI             EXTERN_C __declspec(dllimport) HRESULT STDAPICALLTYPE
	#define DPAl_STDAPI_(type)      EXTERN_C __declspec(dllimport) type STDAPICALLTYPE
#endif //DPAl_EXPORTS


typedef enum DPAl_CREDENTIAL_TYPES {
	DPAl_CREDENTIAL_TYPE_UNKNOWN      =  0x00,  // Unknown type or no authentication at all
	DPAl_CREDENTIAL_TYPE_PASSWORD     =  0x01,  // Password
	DPAl_CREDENTIAL_TYPE_FINGERPRINT  =  0x02,  // Fingerprint
	DPAl_CREDENTIAL_TYPE_SMARTCARD    =  0x04,  // Smart Card
	DPAl_CREDENTIAL_TYPE_FACE         =  0x10,  // Face
	DPAl_CREDENTIAL_TYPE_CONTACTLESS  =  0x20,  // Contactless Card
	DPAl_CREDENTIAL_TYPE_PIN          =  0x80,  // PIN (auxiliary token)
	DPAl_CREDENTIAL_TYPE_PROXIMITY    = 0x100,  // Proximity Card (auxiliary token)
	DPAl_CREDENTIAL_TYPE_BLUETOOTH    = 0x200,  // Bluetooth (auxiliary token)
	DPAl_CREDENTIAL_TYPE_PALM         = 0x400,  // Palm
} DPAl_CREDENTIAL_TYPES;

typedef enum DPAl_USERNAME_TYPES {
    DPAl_USER_NAME_UNKNOWN        = 0, // A name not associated with any Windows account. This value is to be used for local databases only.
    DPAl_USER_NAME_SAM_COMPATIBLE = 3, // A Microsoft Windows NT 4.0 account name, for example, “the_company\jdoe” (only the domain name should end with a back
    DPAl_USER_NAME_USER_PRINCIPAL = 6, // A user principal name, for example, “jdoe@thecompany.com”
	DPAl_USER_NAME_DP_ACCOUNT     = 9, // User name associated with DP identity database.
} DPAl_USERNAME_TYPES;


///////////////////////////////////////////////////////////////////////////////
// DPAlInit
//
// Calling this function is optional. If using multiple calls to other functions in the same application,
// calling DPAlInit once at the beginning speeds up displaying the authentication dialogs and other functions.
// 
// Every call to DPAlInit must be matched by a call to DPAlTerm.
//
// If not called by the application, DPAlInit and DPAlTerm are called implicitly within every function of this SDK,
// but initialization and termination take time, and thus each function will initialize and terminate, resulting
// in duplication of processing and slower performance. If DPAlInit is called once
// at the beginning, all subsequent implicit calls to DPAlInit will return immediately, saving time on subsequent calls.
//
DPAl_STDAPI DPAlInit();

///////////////////////////////////////////////////////////////////////////////
// DPAlTerm
//
// Must be called once for each call of DPAlInit. Normally DPAlInit is called at the beginning of the application, 
// and DPAlTerm is called at the end.
//
DPAl_STDAPI_(VOID) DPAlTerm();

///////////////////////////////////////////////////////////////////////////////
// DPAlAuthenticate
//
// Shows appropriate multi-factor authentication dialog and optionally returns the 
// requested secret upon successful authentication.
//
// Authentication is a 1-to-1 matching process where a user's credentials are compared
// to that user's enrolled credentials stored in the DigitalPersona database.
//
// Returned Values:
//
//   S_OK                           The function succeeded and the user was successfully authenticated
//
//   0x800704c7                     Authentication was canceled by the user
//   HR(ERROR_CANCELLED)
//
//   0x80070002                     Authentication succeeded but the requested secret was not found
//   HR(ERROR_FILE_NOT_FOUND)
//
//   0x80070525                     The username could not be found
//   HR(ERROR_NO_SUCH_USER)
//
//   HR(ERROR_BAD_ENVIRONMENT)      This function was called from a DigitalPersona Kiosk session with the name of the generic Kiosk user. 
//
//   Other COM errors.
//
DPAl_STDAPI DPAlAuthenticate(
	HWND hParentWnd,                   // [in, optional] Handle to the parent window. This parameter can be any valid handle
	                                   // to a window, or it can be NULL if the dialog box has no parent.

	LPCWSTR szCaption,                 // [in, optional] Pointer to a NULL-terminated string that specifies the dialog box
	                                   // caption. If this parameter is NULL, the default caption is displayed.

	LPCWSTR szText,                    // [in, optional] Pointer to a NULL-terminated string that specifies dialog box text.
	                                   // If this parameter is NULL, the default text is displayed.

	LPCWSTR szUserName,                // [in, optional] Pointer to NULL-terminated string with username to authenticate. 
	                                   // It may be in SAM format (i.e. "domain_name\account_name"), UPN format (i.e. "user_name@domain_name.com")
	                                   // or name associated with DigitalPersona identity database (any string). If set to NULL, the currently logged on user is used. 
	                                   // Note that in the DigitalPersona Kiosk environment, the currently logged on user is a generic Kiosk user and cannot be authenticated.

	ULONG uUserNameType,               // [in, optional] Type of szUserName. This parameter must be set to one of DPAl_USER_NAME_SAM_COMPATIBLE, DPAl_USER_NAME_USER_PRINCIPAL 
	                                   // or DPAl_USER_NAME_DP_ACCOUNT respectively. Set it to 0 (DPAl_USER_NAME_UNKNOWN) for currently logged on user.

	UINT64* pPolicyArray,              // [in, optional] Array of custom policy items provided by an application. If NULL, the policy configured by 
	                                   // the Pro administrator is used. 
	                                   // Note that Secrets are released only if the authentication policy configured by the Pro administrator is complied with. 
	                                   // If the application requests to release Secrets, the custom policy should be used only to augment the authentication
	                                   // policy with additional credential.

	ULONG nNumPolicyArrayItems,        // [in, optional] Number of elements in the above array. Must be 0 if pPolicyArray is NULL.

	LPCWSTR szSecretName,              // [in, optional] NULL-terminated string which represents the name of the secret to be retrieved
	                                   // If NULL, no secret is released.

	ULONG* puSecretLength,             // [out, optional] Pointer to variable, receiving length of the secret. Can be NULL if szSecretName is also NULL.

	LPBYTE* ppSecretBuffer,            // [out, optional] Pointer to a buffer which receives secret data. Can be NULL if szSecretName is also NULL.
	                                   // This buffer must be deallocated using the DPAlBufferFree function.

	UINT64* puAuthenticatedCredentials // [out, optional] Pointer to variable receiving mask of credentials used for authentication - one 
	                                   // of the enumerated values from DPAl_CREDENTIAL_TYPES. May be NULL if not needed.
);

///////////////////////////////////////////////////////////////////////////////
// DPAlIdentAuthenticate
//
// Shows appropriate multi-factor authentication dialog (in identification mode), identifies the user 
// and optionally returns requested secret upon successful identification.
//
// Identification is a 1-to-many comparison where the entire Active Directory is searched to find
// the supplied credentials. Once found, the user is authenticated. 
//
// Note: For DigitalPersona Kiosk environments only.  
// If used in Pro Workstation environments, this function only authenticates the currently logged on 
// user, so it provides effectively the same functionality as the DPAlAuthenticate function but with 
// a higher computational cost.
//
// Returned Values:
//
//   S_OK                           The function succeeded and the user was successfully identified
//
//   0x800704c7                     Identification/authentication was canceled by user
//   HR(ERROR_CANCELLED)
//
//   0x80070002                     Identification and authentication succeeded but requested secret was not found
//   HR(ERROR_FILE_NOT_FOUND)
//
//   Other COM errors.
//
DPAl_STDAPI DPAlIdentAuthenticate(
	HWND hParentWnd,                   // [in, optional] Handle to the parent window. This parameter can be any valid handle
	                                   // to a window, or it can be NULL if the dialog box has no parent.

	LPCWSTR szCaption,                 // [in, optional] Pointer to a NULL-terminated string that specifies the dialog box
	                                   // caption. If this parameter is NULL, the default caption is displayed.

	LPCWSTR szText,                    // [in, optional] Pointer to a NULL-terminated string that specifies dialog box text.
	                                   // If this parameter is NULL, the default text is displayed.


	LPCWSTR szDefaultUserName,         // Default username shown in credentials like password or PIN that require a username to be entered by the user. 
	                                   // The user can change the username if needed. 
	                                   // It may be in SAM format (i.e. "domain_name\account_name"), UPN format (i.e. "user_name@domain_name.com")
	                                   // or name associated with DigitalPersona identity database (any string). If set to NULL, the currently logged on user is used. 
	                                   // Set the username to NULL to use the currently logged on user name; set username to empty string (L"") to show no user name.
	                                   // Note that for credentials that perform true identification, like fingerprints, this default username is completely ignored.

	ULONG uDefaultUserNameType,        // Type of szDefaultUserName. This parameter must be set to one of DPAl_USER_NAME_SAM_COMPATIBLE, DPAl_USER_NAME_USER_PRINCIPAL 
	                                   // or DPAl_USER_NAME_DP_ACCOUNT respectively. Set it to 0 (DPAl_USER_NAME_UNKNOWN) for currently logged on user or to set empty szDefaultUserName.

	UINT64* pPolicyArray,              // [in, optional] Array of custom policy items received from an application. If NULL, the policy configured by 
	                                   // the Pro administrator is used. 
	                                   // Note that secrets are released based on the authentication policy configured by the administrator in Pro.
	                                   // If a policy is substituted, the secret may not be released by this function if the user authenticated with less or 
	                                   // other credentials than required to release the secret. 
	                                   // Generally it is advisable to use substituted policies when secret release is not needed 
	                                   // and pdwSecretLength and ppSecretBuffer are set to NULL.

	ULONG nNumPolicyArrayItems,        // [in, optional] Number of elements in the above array. Must be 0 if pPolicyArray is NULL.

	LPCWSTR szSecretName,              // [in, optional] NULL-terminated string which represents the name of the secret need to be retrieved.
	                                   // If NULL, no Secret is released.

	ULONG* pdwSecretLength,            // [out, optional] Pointer to variable, receiving length of the secret. Can be NULL if szSecretName is also NULL.

	LPBYTE* ppSecretBuffer,            // [out, optional] Pointer to a buffer which will receive secret data. Can be NULL if szSecretName is also NULL.
	                                   // This buffer must be deallocated using the DPAlBufferFree function.

	LPWSTR* pszUserName,               // [out, optional] Pointer to a buffer which will receive the name of the authenticated user. Can be NULL if username is not needed.
	                                   // This buffer must be deallocated using DPAlBufferFree function.

	ULONG* puUserNameType,             // [out, optional] Type of pszUserName. Can be one of DPAl_USER_NAME_SAM_COMPATIBLE, DPAl_USER_NAME_USER_PRINCIPAL, or DPAl_USER_NAME_DP_ACCOUNT.
	                                   // Can be NULL if username is not needed.

	UINT64* puAuthenticatedCredentials // [out, optional] Pointer to variable receiving mask of credentials used for authentication - one 
	                                   // of the enumerated values from DPAl_CREDENTIAL_TYPES. May be NULL if not needed.
);

///////////////////////////////////////////////////////////////////////////////
// DPAlReadAuthPolicy
//
// Reads the authentication policy configured by Pro administrator for the specified user on the local computer. 
// This function is mostly needed in case a secret needs to be released with a custom policy, which then
// must be configured to include the items required for secret release.
//
// Return Values:
//
//   S_OK                           The function succeeded and the requested policy was read
//
//   HR(ERROR_BAD_ENVIRONMENT)      This function was called from a DigitalPersona Kiosk session with the name of the generic Kiosk user, 
//                                  or from a non-Kiosk session with the username of user who was not currently logged on (use NULL for the username in this case!)
//   Other COM errors
//
DPAl_STDAPI DPAlReadAuthPolicy(
	LPCWSTR szUserName,             // [in, optional] Pointer to NULL-terminated string with username of the user whose policy is being queried.
	                                // It may be in SAM format (i.e. "domain_name\account_name"), UPN format (i.e. "user_name@domain_name.com")
	                                // or name associated with DigitalPersona identity database (any string). If set to NULL, the currently logged on user is used. 
	                                // Note that in the DigitalPersona Kiosk environment, the currently logged on user is a generic Kiosk user and cannot be authenticated.

	ULONG uUserNameType,            // [in, optional] Type of szUserName. This parameter must be set to one of DPAl_USER_NAME_SAM_COMPATIBLE, DPAl_USER_NAME_USER_PRINCIPAL 
	                                // or DPAl_USER_NAME_DP_ACCOUNT respectively. Set it to 0 (DPAl_USER_NAME_UNKNOWN) for currently logged on user.

	UINT64** pPolicyArray,          // [out] Array of policy items. This array needs to be deallocated by DPAlBufferFree.

	ULONG* pnNumPolicyArrayItems    // [out] Pointer to variable receiving number of elements in the above array.
);


///////////////////////////////////////////////////////////////////////////////
// DPAlWriteSecret
//
// Shows appropriate multi-factor authentication dialog and writes the specified 
// secret content in the Pro database upon successful authentication.
//
// This function can also be used for deleting secrets.
//
// Returned Values:
//
//   S_OK                           The function succeeded and the secret was written
//
//   S_FALSE                        The secret being deleted does not exist
//
//   0x800704c7                     Authentication was canceled by user
//   HR(ERROR_CANCELLED)
//
//   0x80070525                     The username could not be found
//   HR(ERROR_NO_SUCH_USER)
//
//   HR(ERROR_BAD_ENVIRONMENT)      This function was called from a DigitalPersona Kiosk session with the name of the generic Kiosk user, 
//                                  or from a non-Kiosk session with the username of user who was not currently logged on (use NULL for the username in this case!)
//   Other COM errors.
//
DPAl_STDAPI DPAlWriteSecret(
	HWND hParentWnd,                   // [in, optional] Handle to the parent window. This parameter can be any valid handle
	                                   // to a window, or it can be NULL if the dialog box has no parent.

	LPCWSTR szCaption,                 // [in, optional] Pointer to a NULL-terminated string that specifies the dialog box
	                                   // caption. If this parameter is NULL, the default caption is displayed.

	LPCWSTR szText,                    // [in, optional] Pointer to a NULL-terminated string that specifies dialog box text.
	                                   // If this parameter is NULL, the default text is displayed.

	LPCWSTR szUserName,                // [in, optional] Pointer to a NULL-terminated string containing the username of the user whose secret must be written in the database.
	                                   // It may be in SAM format (i.e. "domain_name\account_name"), UPN format (i.e. "user_name@domain_name.com")
	                                   // or name associated with DigitalPersona identity database (any string). If set to NULL, the currently logged on user is used. 
	                                   // Note that in the DigitalPersona Kiosk environment, the currently logged on user is a generic Kiosk user and cannot be authenticated.

	ULONG uUserNameType,               // [in, optional] Type of szUserName. This parameter must be set to one of DPAl_USER_NAME_SAM_COMPATIBLE, DPAl_USER_NAME_USER_PRINCIPAL 
	                                   // or DPAl_USER_NAME_DP_ACCOUNT respectively. Set it to 0 (DPAl_USER_NAME_UNKNOWN) for currently logged on user.

	UINT64* pPolicyArray,              // [in, optional] Array of custom policy items received from an application. If NULL, the policy configured by 
	                                   // the Pro administrator is used. 
	                                   // Note that secrets are saved based on the authentication policy configured by the administrator in Pro.
	                                   // If a policy is substituted, the secret may not be saved by this function if the user authenticated with less or 
	                                   // other credentials than those required to save the secret. 

	ULONG nNumPolicyArrayItems,        // [in, optional] Number of elements in the above array. Must be 0 if pPolicyArray is NULL.

	LPCWSTR szSecretName,              // [in] NULL-terminated string which represents the name of the secret to be written

	ULONG dwSecretLength,              // [in] Size of secret buffer.

	const LPBYTE pSecretBuffer,        // [in] Buffer with secret data to be written. If this parameter is NULL, the secret will be deleted.

	UINT64* puAuthenticatedCredentials // [out, optional] Pointer to variable receiving mask of credentials used for authentication - one 
	                                   // of the enumerated values from DPAl_CREDENTIAL_TYPES. May be NULL if not needed.
);

///////////////////////////////////////////////////////////////////////////////
// DPAlDoesSecretExist
//
// The DPPTDoesSecretExist function checks to see if a given secret exists.
//
// Return Values:
//
//   Returns TRUE if secret exists, FALSE otherwise.
//
DPAl_STDAPI_(BOOL) DPAlDoesSecretExist(
	LPCWSTR szUserName,             // [in, optional] Pointer to a NULL-terminated string containing the username of the user whose secret is being checked.
	                                // It may be in SAM format (i.e. "domain_name\account_name"), UPN format (i.e. "user_name@domain_name.com")
	                                // or name associated with DigitalPersona identity database (any string). If set to NULL, the currently logged on user is used. 
	                                // Note that in the DigitalPersona Kiosk environment, the currently logged on user is a generic Kiosk user and cannot be authenticated.

	ULONG uUserNameType,            // [in, optional] Type of szUserName. This parameter must be set to one of DPAl_USER_NAME_SAM_COMPATIBLE, DPAl_USER_NAME_USER_PRINCIPAL 
	                                // or DPAl_USER_NAME_DP_ACCOUNT respectively. Set it to 0 (DPAl_USER_NAME_UNKNOWN) for currently logged on user.

	LPCWSTR szSecretName            // [in] NULL-terminated string which represents the name of the secret
);

///////////////////////////////////////////////////////////////////////////////
// DPAlBufferFree
//
// The DPAlBufferFree function frees the memory previously allocated by the Pro SDK functions.
//
// Return Values:
//
//   If the function succeeds, the return value is S_OK. If the function fails, the return error is a COM error.
//
DPAl_STDAPI_(void) DPAlBufferFree(
	PVOID pBuffer                   // [in] Buffer to be freed
);

///////////////////////////////////////////////////////////////////////////////
// DPAlFormatMessage
//
// This function retrieves the message text for an error code returned by a function in this SDK or for a standard Windows messages.
// The message text is returned in the language of the current thread.
//
// Return Values:
//
//   If the function succeeds, the return value is S_OK. If the function fails, the return error is a COM error.
//
DPAl_STDAPI DPAlFormatMessage(
	HRESULT hrMessageID,            // [in] ID of the message. This message ID must be in the form of HRESULT. The facility code in it is used
	                                // to load the correct Message DLL, including Windows message DLLs.
	                                
	LPCWSTR szLanguageID,           // [in] Reserved parameter; set it to NULL. In this implementation the language ID of the calling thread is always used.

	LPWSTR* pszMessageText          // [out] NULL-terminated string, text of the message returned from the appropriate Message DLL or System.
	                                // This string must be deallocated with the function DPAlBufferFree.
);


DPAl_STDAPI DPAlVerifyUserUI(HWND hParentWnd, LPCWSTR szCaption, LPCWSTR szText, ULONG hOperation, UINT64* pPolicyArray, ULONG nNumPolicyArrayItems);
DPAl_STDAPI DPAlIdentifyUserUI(HWND hParentWnd, LPCWSTR szCaption, LPCWSTR szText, LPCWSTR szDefaultUserName, ULONG uDefaultUserNameType, UINT64* pPolicyArray, ULONG nNumPolicyArrayItems, LPWSTR* pszUserName, ULONG* uUserNameType, ULONG* phOperation);
DPAl_STDAPI DPAlWriteSecretUI(HWND hParentWnd, LPCWSTR szCaption, LPCWSTR szText, LPCWSTR szUserName, ULONG uUserNameType, UINT64* pPolicyArray, ULONG nNumPolicyArrayItems, LPCWSTR szSecretName, DWORD dwSecretLength, LPBYTE pSecretBuffer, ULONG64* puAuthneticatedCredentials);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DPAl_SDK__
