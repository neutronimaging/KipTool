call set_vars.bat
pushd .

set REPOSPATH=%WORKSPACE%\KipTool
set DEST=%WORKSPACE%\builds
set SPECSTR=-spec win32-msvc

mkdir %DEST%\build-KipTool
cd %DEST%\build-KipTool


%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\applications\kiptool\qt\QtKipTool\QtKipTool.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

popd