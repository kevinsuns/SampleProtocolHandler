// dllmain.h : Deklaration der Modulklasse.

class CSampleProtocolHandlerModule : public CAtlDllModuleT< CSampleProtocolHandlerModule >
{
public :
	DECLARE_LIBID(LIBID_SampleProtocolHandlerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SAMPLEPROTOCOLHANDLER, "{CDB226DF-CB6C-4EA1-88B9-B325E2895309}")
};

extern class CSampleProtocolHandlerModule _AtlModule;
