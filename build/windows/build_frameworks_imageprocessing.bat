call set_vars.bat
pushd .

set REPOSPATH=%WORKSPACE%\KipTool
set DEST=%WORKSPACE%\builds
set SPECSTR=-spec win32-msvc

mkdir %DEST%\build-ProcessFramework
cd %DEST%\build-ProcessFramework

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\ProcessFramework\qt\ProcessFramework\ProcessFramework.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

echo "Build modules"

mkdir %DEST%\build-AdvancedFilterModules
cd %DEST%\build-AdvancedFilterModules

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\AdvancedFilterModules\qt\AdvancedFilterModules\AdvancedFilterModules.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

mkdir %DEST%\build-AdvancedFilterModulesGUI
cd %DEST%\build-AdvancedFilterModulesGUI

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\AdvancedFilterModulesGUI\qt\AdvancedFilterModulesGUI\AdvancedFilterModulesGUI.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release


mkdir %DEST%\build-BaseModules
cd %DEST%\build-BaseModules

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\BaseModules\qt\BaseModules\BaseModules.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

mkdir %DEST%\build-BaseModulesGUI
cd %DEST%\build-BaseModulesGUI

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\BaseModulesGUI\qt\BaseModulesGUI\BaseModulesGUI.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release
echo "Modules built"

mkdir %DEST%\build-ClassificationModules
cd %DEST%\build-ClassificationModules

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\ClassificationModules\qt\ClassificationModules\ClassificationModules.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

mkdir %DEST%\build-ClassificationModulesGUI
cd %DEST%\build-ClassificationModulesGUI

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\ClassificationModulesGUI\qt\ClassificationModulesGUI\ClassificationModulesGUI.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

mkdir %DEST%\build-ImagingModules
cd %DEST%\build-ImagingModules

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\ImagingModules\qt\ImagingModules\ImagingModules.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

mkdir %DEST%\build-ImagingModulesGUI
cd %DEST%\build-ImagingModulesGUI

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\imageprocessing\modules\ImagingModulesGui\qt\ImagingModulesGui\ImagingModulesGui.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

echo "Build tests"

cd %REPOSPATH%\frameworks\imageprocesing\UnitTests

FOR /D %%I IN (*) DO @call :testloopbody %REPOSPATH% %%I %DEST%

echo "Tests built"
popd 

goto :eof

:testloopbody
echo %2
if exist "%1\frameworks\imageprocessing\UnitTests\%2\%2.pro" (
	mkdir %3\%2
	cd %3\%2

	%QTBINPATH%\qmake.exe -makefile %REPOSPATH%\frameworks\imageprocessing\UnitTests\%2\%2.pro -o Makefile
	%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
	%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
	%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release
)
goto :eof
