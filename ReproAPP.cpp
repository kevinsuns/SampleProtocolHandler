// ReproAPP.cpp: Implementierung von CReproAPP

#include "stdafx.h"
#include "ReproAPP.h"
#include <map>
#include <vector>

#pragma warning(disable:4996)

// CReproAPP

// ---- Trace Helper

typedef std::vector<wchar_t> tTraceBuf;

std::map<DWORD, tTraceBuf> MsgBuf;

void ODS(LPCWSTR fmt, ...)
{
#if 1  // enable / disable traces
   tTraceBuf & tb = MsgBuf[GetCurrentThreadId()];
   tb.resize(2048);
   LPWSTR buf = &tb[0];
   LPWSTR target = wcscpy(buf, _T("[RAPP] "));
   target += wcslen(target);

   va_list v;
   va_start(v, fmt);
   wvsprintfW(target, fmt, v);
   va_end(v);
   wcscat(target, _T("\r\n"));
   OutputDebugStringW(buf);
#endif
}




CReproAPP::CReproAPP()
{
   m_file = 0;
   m_sink = 0;
   m_bReadFinished = false;
   m_totalBytesRead = 0;
}

void CReproAPP::ReportResult(HRESULT hr, DWORD code, LPCWSTR msg)
{
   if (m_sink)
   {
      HRESULT hrsink = m_sink->ReportResult(hr, code, msg);
      ODS(L"Reporting result code %Xh: ReportResult returns %Xh", hr, hrsink);
   }
   else
      ODS(L"[RAPP] ERROR: ReportResult has no sink");
      
   ReleaseRessources();
}


void CReproAPP::ReleaseRessources()
{
   if (m_sink != 0)
      m_sink->Release();
   m_sink = 0;
   CloseHandle(m_file);
}



// ---- IInternetProtocol impl
STDMETHODIMP CReproAPP::Read(void *pv,ULONG cb, ULONG *pcbRead)
{
   if (m_bReadFinished)
      return S_FALSE;

   if (!m_file || !pv || !pcbRead)
   {
      ODS(L"Read: ERROR - Unexpected call");
      return E_UNEXPECTED;
   }

   DWORD didRead = 0;
   BOOL ok = ReadFile(m_file, pv, cb, &didRead, NULL);
   if (!ok)
   {
      DWORD err = GetLastError();
      ODS(L"Read: ReadFile: ERROR %Xh after %I64d bytes read", err, m_totalBytesRead);

      ReportResult(HRESULT_FROM_WIN32(err), -1);
      return HRESULT_FROM_WIN32(err);
   }

   *pcbRead = didRead;
   m_totalBytesRead += didRead;

   // Read completely?
   if (didRead == 0)
   {
      ODS(L"Read complete (%I64d bytes total), returning S_FALSE", m_totalBytesRead);

      m_bReadFinished = true;
      m_sink->ReportData(BSCF_LASTDATANOTIFICATION, 0, 0);
      ReportResult(S_OK);
      return S_FALSE;
   }
   return S_OK;
}

extern HINSTANCE g_instanceDLL;  // initialized in DLLMain

STDMETHODIMP CReproAPP::Start(LPCWSTR szUrl, IInternetProtocolSink *pOIProtSink, IInternetBindInfo *pOIBindInfo, DWORD grfPI, HANDLE_PTR dwReserved)
{
   LPCWSTR prefix = _T("klrepro://");
   const unsigned prefixLen = wcslen(prefix);

   if (m_sink || 
       !pOIProtSink || !szUrl || 
       wcslen(szUrl) < prefixLen || wcsncmp(prefix, szUrl, prefixLen) != 0 ||
       !g_instanceDLL)  // not first call, or missing or invalid parameters
   {
      ODS(L"Start: ERROR - Unexpected call");
      return E_UNEXPECTED;
   }

   ODS(L"Start: url=%s", szUrl);

   // --- find file to use (local file in DLL folder)
   // (might end up with wrong file name if buffer exceeded, no prob for repro)
   wchar_t filePath[_MAX_FNAME+1] = { 0 };
   DWORD pathLen = _MAX_FNAME;
   GetModuleFileNameW(g_instanceDLL, filePath, _MAX_FNAME);
   LPWSTR p = PathFindFileNameW(filePath);
   wcsncpy(p, szUrl+prefixLen, _MAX_FNAME-(p-filePath));
   const int len = wcslen(filePath);
   for(int i=0; i<len; ++i)
      if (filePath[i] == '/')
         filePath[i] = '\\';

   // --- use file
   m_file = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
   if (m_file == INVALID_HANDLE_VALUE)
   {
      m_file = 0;
      DWORD err = GetLastError();
      ODS(L"Read: Error %Xh opening file '%s'", err, filePath);
      return E_UNEXPECTED;
   }

   m_sink = pOIProtSink;
   m_sink->AddRef();

   // enabling either of this mime types does not help for .swf files, but ...
   // LPCWSTR mimeType = L"application/octet-stream";       // before FIRSTDATANOTIFICATION: no download, after: behavior unchanged
   // LPCWSTR mimeType = L"application/x-shockwave-flash";  // before, after FIRSTDATANOTIFICATION: behavior unchanged
   // m_sink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, mimeType);

   m_sink->ReportData(BSCF_FIRSTDATANOTIFICATION, 0, 0);

   return S_OK;
}

// ---- Methods that are not implemented

STDMETHODIMP CReproAPP::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER * plibNewPosition)
{
   ODS(L"Seek: E_NOTIMPL");
   return E_NOTIMPL;
}

STDMETHODIMP CReproAPP::LockRequest(DWORD dwOptions)
{
   ODS(L"LockRequest: no-op, returning S_OK");
   return S_OK;
}

STDMETHODIMP CReproAPP::UnlockRequest()
{
   ODS(L"UnlockRequest: no-op, returning S_OK");
   return S_OK;
}



STDMETHODIMP CReproAPP::Continue(PROTOCOLDATA *pProtocolData)
{
   ODS(L"Continue: returning S_OK");
   return S_OK;
}

STDMETHODIMP CReproAPP::Abort(HRESULT hrReason, DWORD dwOptions)
{
   ODS(L"Abort");
   ReleaseRessources();
   return S_OK;
}

STDMETHODIMP CReproAPP::Terminate(DWORD dwOptions)
{
   ODS(L"Terminate");
   ReleaseRessources();
   return S_OK;
}

STDMETHODIMP CReproAPP::Suspend()
{
   ODS(L"Suspend: returning E_NOTIMPL");
   return E_NOTIMPL;
}

STDMETHODIMP CReproAPP::Resume()
{
   ODS(L"Result");
   return S_OK;
}
