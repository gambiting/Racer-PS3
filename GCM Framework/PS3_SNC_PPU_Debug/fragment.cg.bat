@echo off
@echo Performing Custom Build Step
C:/usr/local/cell\host-win32\Cg\bin\sce-cgc -profile sce_fp_rsx -o "fragment.fpo" "d:\Team Project\Racer-PS3\GCM Framework\fragment.cg"
C:/usr/local/cell\host-win32\Cg\bin\sce-cgcdisasm "fragment.fpo"
if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: A tool returned an error code from "Performing Custom Build Step"
exit 1
:VCEnd
