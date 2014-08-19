// SampleProtocolHandler.cpp : Implementierung von DLL-Exporten.


#include "stdafx.h"
#include "resource.h"
#include "SampleProtocolHandler_i.h"
#include "dllmain.h"

// Wird verwendet, um festzustellen, ob die DLL von OLE entladen werden kann.
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Gibt eine Klassenfactory zurück, um ein Objekt vom angeforderten Typ zu erstellen.
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Fügt der Systemregistrierung Einträge hinzu.
STDAPI DllRegisterServer(void)
{
    // Registriert Objekt, Typelib und alle Schnittstellen in Typelib.
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Entfernt Einträge aus der Systemregistrierung.
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

// DllInstall - Fügt der Systemregistrierung pro Benutzer Einträge hinzu oder entfernt sie.
//              pro Computer.	
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
    HRESULT hr = E_FAIL;
    static const wchar_t szUserSwitch[] = _T("user");

    if (pszCmdLine != NULL)
    {
    	if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
    	{
    		AtlSetPerUserRegistration(true);
    	}
    }

    if (bInstall)
    {	
    	hr = DllRegisterServer();
    	if (FAILED(hr))
    	{	
    		DllUnregisterServer();
    	}
    }
    else
    {
    	hr = DllUnregisterServer();
    }

    return hr;
}


