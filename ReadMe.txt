 - ATL project (DLL) with default settings
 - Added default ATL object (Agrregatable, Dual itf)
 - removed custom interface from IDL, impl
 - added "klrepo" protocol to ReproAPP.rgs
 - registered DLL (admin)
 - Added IInternetProtocol(Root) implementation, as required
 
 
 Tests:
 
 klrepro://<relative path> opens files relative to the DLL path. 
 
 Test URLs: 
 
 klrepro://html/img.jpg       shows the image, OK
 klrepro://html/index.html    shows HTML and the image referenced, OK
 
 klrepro://swf/small.swf      
 
   Shows a Download Dialog (Well, OK)
 
         [Window Title]
         Internet Explorer

         [Main Instruction]
         Wie möchten Sie mit „small_swf“ verfahren?

         [Content]
         Von: swf

         [Öffnen] [Speichern] [Speichern unter] [Abbrechen]
 
 (Note the suggested name "small_swf")
 
 Choosing "Save as" does retrieve the entire file through the APP (as seen in the DebugLog)
 but ends with 
 
      "small_swf konnte nicht heruntergeladen werden."

meaning: Could not download "small_swf" 

Size of the ".partial" files remaining is 0 bytes. 


The DebugLog shows 

 - calls to the APP are the same as for file types where it's working. 
 - The only methods called are Start, Read, LockRequest, UnlockRequest, Terminate
 - IInternetProtocolSink::ReportResult succeeds
 
[12984] [RAPP] Start: url=klrepro://swf/small.swf 
[12984] [RAPP] LockRequest: no-op, returning S_OK 
[12984] [RAPP] Read complete (35084 bytes total), returning S_FALSE 
[12984] [RAPP] Reporting result code 0h: ReportResult returns 0h 
[12984] [RAPP] Terminate 
[12984] [RAPP] UnlockRequest: no-op, returning S_OK 
 

I've tried: 

 - implementing IInternetProtocolInfo
   (reference is thin, no real samples I could find)
   
 - reporting mime type through BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE
   (application/
   
 
 - ReportData for progress makes no difference  
 - reporting more through sink.ReportProgress, using the sparse samples that do

 - Same behavior occurs for a file with a completely unknown file extension
   (I'd expect / hope for a "pure" download)