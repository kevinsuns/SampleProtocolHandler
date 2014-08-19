// ReproAPP.h: Deklaration von CReproAPP

#pragma once
#include "resource.h"       // Hauptsymbole

#include "SampleProtocolHandler_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Singlethread-COM-Objekte werden auf der Windows CE-Plattform nicht vollständig unterstützt. Windows Mobile-Plattformen bieten beispielsweise keine vollständige DCOM-Unterstützung. Definieren Sie _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, um ATL zu zwingen, die Erstellung von Singlethread-COM-Objekten zu unterstützen und die Verwendung eigener Singlethread-COM-Objektimplementierungen zu erlauben. Das Threadmodell in der RGS-Datei wurde auf 'Free' festgelegt, da dies das einzige Threadmodell ist, das auf Windows CE-Plattformen ohne DCOM unterstützt wird."
#endif


class __declspec(uuid("d334f580-7eba-4910-a856-b40933811d42")) CReproAPP;

// CReproAPP

class ATL_NO_VTABLE CReproAPP :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CReproAPP, &__uuidof(CReproAPP)>,
   public IInternetProtocol
{
public:
	CReproAPP();

DECLARE_REGISTRY_RESOURCEID(IDR_REPROAPP)


BEGIN_COM_MAP(CReproAPP)
   COM_INTERFACE_ENTRY(IInternetProtocol)
   COM_INTERFACE_ENTRY(IInternetProtocolRoot)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
   // IInternet Protocol
   STDMETHOD(Read)(void *pv,ULONG cb, ULONG *pcbRead);
   STDMETHOD(Seek)( LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER * plibNewPosition);
   STDMETHOD(LockRequest)(DWORD dwOptions);
   STDMETHOD(UnlockRequest)();

   // IInternet ProtocolRoot
   STDMETHOD(Start)( LPCWSTR szUrl, IInternetProtocolSink *pOIProtSink, IInternetBindInfo *pOIBindInfo, DWORD grfPI, HANDLE_PTR dwReserved);
   STDMETHOD(Continue)(PROTOCOLDATA *pProtocolData);
   STDMETHOD(Abort)(HRESULT hrReason, DWORD dwOptions);

   STDMETHOD(Terminate)(DWORD dwOptions);
   STDMETHOD(Suspend)();
   STDMETHOD(Resume)();

protected:

   void ReportResult(HRESULT hr, DWORD code = 0, LPCWSTR msg = 0);
   void ReleaseRessources();
   HANDLE   m_file;
   IInternetProtocolSink * m_sink;
   bool m_bReadFinished;
   __int64 m_totalBytesRead;

};

OBJECT_ENTRY_AUTO(__uuidof(CReproAPP), CReproAPP)
