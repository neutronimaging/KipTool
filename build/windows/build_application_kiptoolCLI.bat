call set_vars.bat
pushd .

set REPOSPATH=%WORKSPACE%\KipTool
set DEST=%WORKSPACE%\builds
set SPECSTR=-spec win32-msvc

mkdir %DEST%\build-kiptoolCLI
cd %DEST%\build-kiptoolCLI


%QTBINPATH%\qmake.exe -makefile ..\..\KipTool\applications\kiptoolCLI\QtKipToolCLI\QtKipToolCLI.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom.exe -f Makefile clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom.exe -f Makefile mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom.exe -f Makefile release

popd 



