@echo off
@echo Performing Custom Build Step
C:/usr/local/cell\host-win32\Cg\bin\sce-cgc -profile sce_vp_rsx -o "vertex.vpo" "d:\Dev\FGRacer\Racer-PS3\GCM Framework\vertex.cg"
if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: A tool returned an error code from "Performing Custom Build Step"
exit 1
:VCEnd
