//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// ************************************************************
// SmsRule.CPP
// 
// Sample implementation of a message Rule Client
//
//


#define INITGUID
#include "windows.h"
#include "cemapi.h"

int g_cServerLocks = 0;

// {A9AA8987-3550-49d2-866E-5931265726AA}
DEFINE_GUID(CLSID_SmsRuleSpamProtector, 
			0xa9aa8987, 0x3550, 0x49d2, 0x86, 0x6e, 0x59, 0x31, 0x26, 0x57, 0x26, 0xaa);

// **************************************************************************
// Function Name: DeleteMessage
//
// Purpose: Delete a MAPI message

// Arguments:
//		IN IMsgStore* pMsgStore - Message Store to delete from
//		IN IMessage* pMsg - ptr to message to be deleted
//		IN ULONG cbMsg - The size of lpMsg, in bytes
//		IN LPENTRYID lpMsg - The ENTRYID of the message
//		IN ULONG cbDestFolder - The size of lpDestFolder, in bytes
//		IN LPENTRYID lpDestFolder - The ENTRYID of the folder that incoming
//			messages are moved to
//		OUT ULONG *pulEventType - Combination of bit flags indicating the type if 
//			action performed on the message, deletion in this case
//		OUT MRCHANDLED *pHandled - Indicates the type of handling that occurred,
//			in this case we mark the message as handled, and do not pass it on 
//
// Return Values:  HRESULT depending on success of MAPI operations

// Side effects:  

// Description:  
//	This function deletes a given message from a given folder and 
//  sets the proper notification event  

HRESULT DeleteMessage(IMsgStore *pMsgStore, 
					  IMessage *pMsg, 
					  ULONG cbMsg, 
					  LPENTRYID lpMsg,
					  ULONG cbDestFolder, 
					  LPENTRYID lpDestFolder, 
					  ULONG *pulEventType, 
					  MRCHANDLED *pHandled)
{
	HRESULT hr = S_OK;
	ENTRYLIST lst;
	SBinary sbin;
	IMAPIFolder *pFolder = NULL;

	// Delete it
	hr = pMsgStore->OpenEntry(cbDestFolder, lpDestFolder, NULL, 0, NULL, (LPUNKNOWN *) &pFolder);
	if (FAILED(hr))
	{
		RETAILMSG(TRUE, (TEXT("Couldn't get the folder!\r\n")));
		goto Exit;
	}

	lst.cValues = 1;
	sbin.cb = cbMsg;
	sbin.lpb = (LPBYTE) lpMsg;
	lst.lpbin = &sbin;

	hr = pFolder->DeleteMessages(&lst, NULL, NULL, 0); 
	if (FAILED(hr))
	{
		RETAILMSG(TRUE, (TEXT("Couldn't delete messages!\r\n")));
		goto Exit;
	}

	// Notification object lets listeners know we deleted this
	*pulEventType = fnevObjectDeleted;
	// Mark as handled and don't pass on
	*pHandled = MRC_HANDLED_DONTCONTINUE;

Exit:
	if (pFolder)
	{
		pFolder->Release();
	}

	return hr;
}

// ************************************************************
// Class CMailRuleClient - Implementation of IMailRuleClient
//
// Inheritance:
//     IMailRuleClient IUnknown (Abstract)
//
// Purpose:
//     This class serves as implementation for the IMailRuleClient
//	   interface and provides our Rule Client functionality.
//	   The Initialize method sets our permissions to interact
//	   with the message store, and the ProcesseMessage method
//	   defines how we handle incoming messages
//
// ************************************************************

class CMailRuleClient : public IMailRuleClient
{
public:
	CMailRuleClient();
	~CMailRuleClient();

	// IUnknown
	STDMETHOD (QueryInterface)(REFIID iid, LPVOID *ppv);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IMailRuleClient
	MAPIMETHOD(Initialize)(
		IMsgStore *pMsgStore,
		MRCACCESS *pmaDesired
		);

	MAPIMETHOD(ProcessMessage)(
		IMsgStore *pMsgStore, 
		ULONG cbMsg,
		LPENTRYID lpMsg,
		ULONG cbDestFolder,
		LPENTRYID lpDestFolder,
		ULONG *pulEventType,
		MRCHANDLED *pHandled
		);

private:
	long m_cRef;
};


// ************************************************************
// Class CFactory - Class factory for CMailRuleClient objects
//
// Inheritance:
//     IClassFactory IUnknown
//
// Purpose:
//     This class provides a standard COM class factory implementation
//	   for CMailRuleClient
//
// ************************************************************
class CFactory : public IClassFactory
{
public:
	CFactory();
	~CFactory();

	// IUnknown
	STDMETHOD (QueryInterface)(REFIID iid, LPVOID *ppv);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IClassFactory interfaces
	STDMETHOD (CreateInstance)(IUnknown *pUnknownOuter, const IID& iid, LPVOID *ppv);
	STDMETHOD (LockServer)(BOOL bLock);

private:
	long m_cRef;
};

// **************************************************************************
// Function Name: CFactory 

// Purpose: Initializes CFactory object

// Arguments:

// Return Values:

// Side effects:  

// Description: 
//	Constructor for CFactory class. Initializes class members.

CFactory::CFactory()
{
	m_cRef = 1;
}

// **************************************************************************

// Function Name: ~CFactory
//
// Purpose: Cleans up CFactory object

// Arguments:

// Return Values:

// Side effects:  

// Description:  
//	Destructor for CFactory object

CFactory::~CFactory()
{
}

// **************************************************************************
// Function Name: QueryInterface
// Purpose: Obtains caller's desired interface pointer if it is supported

// Arguments:
//	IN IID& iid - Identifier for desired interface
//	OUT LPVOID *ppv - pointer to desired interface pointer

// Return Values: HRESULT 
//	E_NOINTERFACE:  the requested interface is not supported
//	E_INVALIDARG: bad reference for out param

// Side effects:  

// Description:
//	Standard implementation of COM IUnknown::QueryInterface

STDMETHODIMP CFactory::QueryInterface(const IID& iid, LPVOID *ppv)
{
	HRESULT hr = E_NOINTERFACE;

	if (!ppv)
	{
		return E_INVALIDARG;
	}

	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = (LPVOID) this;
	}
	else
	{
		*ppv = NULL;
	}

	if (*ppv) 
	{
		((LPUNKNOWN) *ppv)->AddRef();
		hr = S_OK;
	}

	return hr;
}

// **************************************************************************
// Function Name: AddRef
// Purpose: COM reference counting

// Description:  
//	Implements IUnknown::Addref by adding 1 to the object's reference count
ULONG CFactory::AddRef()
{
	RETAILMSG(TRUE, (TEXT("Factory Reference is now %d\r\n"), m_cRef + 1));
	return InterlockedIncrement(&m_cRef);
}

// **************************************************************************
// Function Name: Release
// Purpose: COM reference counting

// Return Value: ULONG - reference count after decrementing 

// Description:  
//	Implements IUnknown::Release by subtracting 1 from the object's reference count
ULONG CFactory::Release()
{
	InterlockedDecrement(&m_cRef);
	RETAILMSG(TRUE, (TEXT("Factory Reference is now %d\r\n"), m_cRef));

	int nLocal = m_cRef;

	if (!m_cRef) 
	{
		RETAILMSG(TRUE, (TEXT("CFactory Deleted!\r\n")));
		delete this; 
	}

	return nLocal; 
}

// **************************************************************************
// Function Name: CreateInstance

// Purpose: Create a new instance of a COM object and return the specified
//	interface

// Arguments:	IN LPUNKNOWN pUnknownOuter - controlling outer for aggregation
//				IN REFIID iid - interface identifier GUID reference
//				OUT LPVOID *ppv - pointer to newly created interface pointer

// Return Values: HRESULT, S_OK if successful, error otherwise

// Side effects:  

// Description:  
STDMETHODIMP CFactory::CreateInstance(LPUNKNOWN pUnknownOuter, REFIID iid, LPVOID *ppv)
{
	CMailRuleClient *pClient = NULL;
	HRESULT hr;

	// No aggregation
	if (pUnknownOuter)
	{
		hr = CLASS_E_NOAGGREGATION;
		goto Error;

	}

	// You heard 'em, create a component
	pClient = new CMailRuleClient();
	if (!pClient)
	{
		hr = E_OUTOFMEMORY;
		goto Error;
	}

	// Get the requested interface
	hr = pClient->QueryInterface(iid, ppv);

Error:
	// Release the unknown pointer
	if (pClient) 
	{
		pClient->Release();
	}

	return hr;
}

// **************************************************************************
// Function Name: LockServer
// Purpose: Increment or decrement the number of lock on a COM server

// Arguments: IN BOOL bLock - increment(TRUE) or decrement(FALSE) the lockcount

// Return Values: HRESULT - S_OK

// Side effects:  

// Description:  
STDMETHODIMP CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		g_cServerLocks++;
	}
	else
	{
		g_cServerLocks--;
	}

	return S_OK;
}


// **************************************************************************
// Function Name: CMailRuleClient
//
// Purpose: Initialize the CMailRuleClient object

// Arguments:	none

// Return Values:	none

// Side effects:  

// Description: CMailRuleClient Constructor 
CMailRuleClient::CMailRuleClient()
{
	m_cRef = 1;
}

// **************************************************************************
// Function Name: ~CMailRuleClient
//
// Purpose: Cleans up CMailRuleClient object

// Arguments:

// Return Values:

// Side effects:  

// Description:  
//	Destructor for CMailRuleClient object
CMailRuleClient::~CMailRuleClient()
{
}

// **************************************************************************
// Function Name: QueryInterface
// Purpose: Obtains caller's desired interface pointer if it is supported

// Arguments:
//	IN IID& iid - Identifier for desired interface
//	OUT LPVOID *ppv - pointer to desired interface pointer

// Return Values: HRESULT 
//	E_NOINTERFACE:  the requested interface is not supported
//	E_INVALIDARG: bad reference for out param

// Side effects:  

// Description:
//	Standard implementation of COM IUnknown::QueryInterface

HRESULT CMailRuleClient::QueryInterface(REFIID rif, void** ppobj)
{	
	HRESULT hr = E_NOINTERFACE;

	if (!ppobj)
	{
		return E_INVALIDARG;
	}

	*ppobj = NULL;
	if ((rif == IID_IUnknown) || (rif == IID_IMailRuleClient))
	{
		*ppobj = (LPVOID) this;
	}

	if (*ppobj) 
	{
		((LPUNKNOWN) *ppobj)->AddRef();
		hr = S_OK;
	}

	return hr;
}

// **************************************************************************
// Function Name: AddRef

// Purpose: COM reference counting

// Arguments: none

// Return Values: current ref count (after adding)

// Description:  
//	Implements IUnknown::Addref by adding 1 to the object's reference count
ULONG CMailRuleClient::AddRef()
{
	RETAILMSG(TRUE, (TEXT("CMailRuleClient reference is now %d\r\n"), m_cRef + 1));
	return InterlockedIncrement(&m_cRef);
}

// **************************************************************************
// Function Name: Release
// Purpose: COM reference counting

// Arguments: none

// Return Values: current ref count (after subracting)

// Side effects:  

// Description:
//	Implements IUnknown::Addref by subtracting 1 from the object's reference count  

ULONG CMailRuleClient::Release()
{
	InterlockedDecrement(&m_cRef);
	RETAILMSG(TRUE, (TEXT("CMailRuleClient reference is now %d\r\n"), m_cRef));

	int nLocal = m_cRef;

	if (!m_cRef) 
	{
		RETAILMSG(TRUE, (TEXT("CMailRuleClient Deleted!\r\n")));
		delete this; 
	}

	return nLocal; 
}

// **************************************************************************
// Function Name: Initialize
//
// Purpose: determines how the mail rule client will process incoming messages.

// Arguments: IN IMsgStore * pMsgStore - represests message store which contains
//	the incoming messages
//			  OUT MRCACCESS *pmaDesired - desired message store access level	

// Return Values: HRESULT - S_OK

// Side effects:  

// Description:  
//	This function is called by the system to initialize rule clients. Since we
//	eventually may want to delete messages, we request write access here
HRESULT CMailRuleClient::Initialize(IMsgStore *pMsgStore, MRCACCESS *pmaDesired)
{

	*pmaDesired = MRC_ACCESS_WRITE;
	return S_OK;
}

// **************************************************************************
// Function Name: ProcessMessage
//
// Purpose: process incoming messages, which can be moved, modified, or deleted 

// Arguments: IN IMsgStore * pMsgStore - represests message store which contains
//				the incoming messages
//			  IN ULONG cbMsg - The size of lpMsg in bytes
//			  IN LPENTRYID - The ENTRYID of the message
//			  IN ULONG cbDestFolder - The size of lpDestFolder in bytes
//			  IN LPENTRYID lpDestFolder - The ENTRYID of the the folder that 
//				incoming messages are moved to.
//			  OUT ULONG * pulEventType - bit flag that indicates the type of 
//				action the client performed on the message 
//			  OUT MRCHANDLED * pHandled - The type of message handling that 
//				occured during the processing
//			  

// Return Values: HRESULT 
//	This method returns S_OK if the processing was successful, and appropriate 
//	errors if not. 

// Side effects:  

// Description: 
//	This function is called by the system when an incoming message is received
//  This is where all the plugin-defined processing happens. You can filter 
//	messages based on content, delete or move messages, and report whether the
//  message has been handled or not.

HRESULT CMailRuleClient::ProcessMessage(IMsgStore *pMsgStore, ULONG cbMsg, LPENTRYID lpMsg, 
										ULONG cbDestFolder, LPENTRYID lpDestFolder, ULONG *pulEventType, MRCHANDLED *pHandled)
{
	HRESULT hr = S_OK;
	SizedSPropTagArray(1, sptaSubject) = { 1, PR_SUBJECT}; 
	SizedSPropTagArray(1, sptaEmail) = { 1, PR_SENDER_EMAIL_ADDRESS}; 
	ULONG cValues = 0;
	SPropValue *pspvSubject = NULL;
	SPropValue *pspvEmail = NULL;
	IMessage *pMsg = NULL;
	HRESULT hrRet = S_OK;

	// Get the message from the entry ID
	hr = pMsgStore->OpenEntry(cbMsg, lpMsg, NULL, 0, NULL, (LPUNKNOWN *) &pMsg);
	if (FAILED(hr))
	{

		RETAILMSG(TRUE, (TEXT("Unable to get the message!\r\n")));
		goto Exit;
	}

	// For SMS, the subject is also the message body
	hr = pMsg->GetProps((SPropTagArray *) &sptaSubject, MAPI_UNICODE, &cValues, &pspvSubject);
	if (FAILED(hr))
	{

		RETAILMSG(TRUE, (TEXT("Unable to get the message body!\r\n")));
		goto Exit;
	}
	// get the sender's address or phone number
	hr = pMsg->GetProps((SPropTagArray *) &sptaEmail, MAPI_UNICODE, &cValues, &pspvEmail);

	if (FAILED(hr))
	{

		RETAILMSG(TRUE, (TEXT("Couldn't get the sender's address!\r\n")));

		goto Exit;
	}

	// Here we filter the message on some predetermined string. For sample purposes
	// here we use "zzz". What happens when the filter condition(s) are met is up to
	// you. You can send WM_COPYDATA messages to other app windows for light IPC, send
	// an SMS message in response, or whatever you need to do. Here, we just play a
	// sound and show the message in a standard message box.
	if (wcsstr(pspvSubject->Value.lpszW, L"zzz") != NULL)

	{
		MessageBeep(MB_ICONASTERISK);
		MessageBox(NULL, pspvSubject->Value.lpszW, pspvEmail->Value.lpszW, MB_OK);

		// Delete the message and mark it as handled so it won't show up in Inbox
		hr = DeleteMessage(pMsgStore, pMsg, cbMsg, lpMsg, cbDestFolder, lpDestFolder, pulEventType, pHandled);
	}
	else 
	{
		// a 'normal' message, pass it on
		*pHandled = MRC_NOT_HANDLED;	
	}

	// Clean up
Exit:
	if (pspvEmail)
	{
		MAPIFreeBuffer(pspvEmail);
	}
	if (pspvSubject)
	{
		MAPIFreeBuffer(pspvSubject);
	}
	if (pMsg)
	{
		pMsg->Release();
	}


	return hr;
}

// **************************************************************************
// Function Name: DllMain
// Purpose: DLL Entry point

// Arguments: IN HANDLE hinst - Handle to the DLL
//			  IN DWORD dwReason - flag indicating why entry-point was called
//			  IN LPVOID lpv - specifies further aspects of initialization and
//				cleanup 

// Return Values: TRUE if initialization succeeds, FALSE otherwise

// Side effects:  

// Description: 
//	Called by system when a thread or process loads/unloads the dll 
BOOL WINAPI DllMain(HANDLE hinst, DWORD dwReason, LPVOID lpv)
{


	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_PROCESS_DETACH:
		break;

	default:
		break;
	}

	return TRUE;
}

// **************************************************************************
// Function Name: DllGetClassObject
// Purpose: Retrieves the class object from the DLL object

// Arguments: IN CLSID& clsid - CLSID for desired class object
//			  IN REFIID iid - ref to interface id, usually IClassFactory
//			  OUT LPVOID *ppv - address of requested interface pointer

// Return Values: HRESULT

// Side effects:  

// Description:  
STDAPI DllGetClassObject(const CLSID& clsid, REFIID iid, LPVOID *ppv)
{
	HRESULT hr;

	// We only understand this rule's class
	if (clsid != CLSID_SmsRuleSpamProtector)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	CFactory *pFactory = new CFactory;
	if (!pFactory)
	{
		return E_OUTOFMEMORY;
	}

	// Get the requested interface
	hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}

// **************************************************************************
// Function Name: DllCanUnloadNow
//
// Purpose: Notify caller if dll can safely be unloaded

// Arguments: none

// Return Values: HRESULT, S_OK if safe to unload, S_FALSE otherwise

// Side effects:  

// Description:  A call to DllCanUnloadNow determines whether the DLL from 
//	which it is exported is still in use. A DLL is no longer in use when it 
//	is not managing any existing objects (the reference count on all of its 
//	objects is zero). 

STDAPI DllCanUnloadNow()
{
	if (!g_cServerLocks)
	{

		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

// **************************************************************************
// Function Name: DllRegisterServer
//
// Purpose: provide DLL with the ability to register its COM objects

// Arguments: none

// Return Values: HRESULT - S_OK if registration succeeds, E_FAIL otherwise

// Side effects: In order to fully remove the plugin, both registry keys should
//	be removed (see Description below). DllUnregisterServer does this.

// Description:  
//  In addition to standard COM object registration, the function also must
//  register our rule client handler with Inbox.  We are registering our 
//	DLL in two places, HKEY_CLASSES_ROOT\CLSID\, and HKEY_LOCAL_MACHINE\Software
//	\Microsoft\Inbox\Svc\SMS\Rules
STDAPI DllRegisterServer()
{
	LRESULT lr;
	HRESULT hr = E_FAIL;
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;
	DWORD dwDisposition;
	TCHAR wszValue[20];

	// Set up registry keys
	// Register with COM:
	//    [HKEY_CLASSES_ROOT\CLSID\{A9AA8987-3550-49d2-866E-5931265726AA}\InProcServer32]
	//    @="SmsRule.dll"


	lr = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT("\\CLSID\\{A9AA8987-3550-49d2-866E-5931265726AA}"),
		0, NULL, 0, 0, NULL, 
		&hKey, &dwDisposition);
	if (lr != ERROR_SUCCESS)
	{
		goto Exit;
	}

	lr = RegCreateKeyEx(hKey, TEXT("InprocServer32"),
		0, NULL, 0, 0, NULL, 
		&hSubKey, &dwDisposition);
	if (lr != ERROR_SUCCESS)
	{
		goto Exit;
	}

	lstrcpy(wszValue, TEXT("SmsRule.dll"));
	lr = RegSetValueEx(hSubKey, NULL, 0, REG_SZ, (LPBYTE) wszValue, (lstrlen(wszValue) + 1) * sizeof(TCHAR));
	if (lr != ERROR_SUCCESS)
	{
		goto Exit;
	}

	RegCloseKey(hSubKey);
	hSubKey = NULL;
	RegCloseKey(hKey);
	hKey = NULL;

	// Register with Inbox:
	//    [HKEY_LOCAL_MACHINE\Software\Microsoft\Inbox\Svc\SMS\Rules]
	//    {A9AA8987-3550-49d2-866E-5931265726AA}"=dword:1

	lr = RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("\\Software\\Microsoft\\Inbox\\Svc\\SMS\\Rules"),
		0, NULL, 0, 0, NULL, 
		&hKey, &dwDisposition);
	if (lr != ERROR_SUCCESS)
	{
		goto Exit;
	}

	dwDisposition = 1;
	lr = RegSetValueEx(hKey, TEXT("{A9AA8987-3550-49d2-866E-5931265726AA}"), 0, REG_DWORD, 
		(LPBYTE) &dwDisposition, sizeof(DWORD));
	if (lr != ERROR_SUCCESS)
	{
		goto Exit;
	}

	hr = S_OK;

Exit:
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
	}
	if (hKey)
	{
		RegCloseKey(hKey);
	}

	return hr;
}

// **************************************************************************
// Function Name: DllUnregisterServer
// Purpose: rovide DLL with the ability to un-register its COM objects


// Arguments: none

// Return Values: HRESULT - S_OK if registration succeeds, E_FAIL otherwise

// Side effects: 

// Description:  Deletes both the COM registry key and the key used to register
// the rule client with Inbox
STDAPI DllUnregisterServer()
{
	HKEY hKey = NULL;
	HRESULT hr = E_FAIL;
	LRESULT lr;
	DWORD dwDisposition;

	// Delete registry keys
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("\\CLSID\\{A9AA8987-3550-49d2-866E-5931265726AA}"));

	lr = RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("\\Software\\Microsoft\\Inbox\\Svc\\SMS\\Rules"),
		0, NULL, 0, 0, NULL, 
		&hKey, &dwDisposition);
	if (lr != ERROR_SUCCESS)
	{
		goto Exit;
	}

	RegDeleteValue(hKey, TEXT("{A9AA8987-3550-49d2-866E-5931265726AA}"));

	hr = S_OK;

Exit:
	if (hKey)
	{
		RegCloseKey(hKey);
	}

	return hr;
}

