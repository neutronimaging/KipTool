set REPOS=%WORKSPACE%
set DEST=%WORKSPACE%\deployed\kiptool

IF NOT EXIST %DEST% mkdir %DEST%
pushd .
cd %DEST%

copy %REPOS%\build-kiptool\bin\Release\KipTool.exe .
@REM copy %REPOS%\Applications\KipToolCLI.exe .
copy %REPOS%\build-imagingsuite\bin\Release\ImagingAlgorithms.dll .
copy %REPOS%\build-imagingsuite\bin\Release\ModuleConfig.dll .
copy %REPOS%\build-imagingsuite\bin\Release\ReaderConfig.dll .
copy %REPOS%\build-imagingsuite\bin\Release\ReaderGUI.dll .
copy %REPOS%\build-imagingsuite\bin\Release\QtAddons.dll .
copy %REPOS%\build-imagingsuite\bin\Release\QtModuleConfigure.dll .
copy %REPOS%\build-imagingsuite\bin\Release\kipl.dll .
copy %REPOS%\build-kiptool\bin\Release\ProcessFramework.dll .
copy %REPOS%\build-kiptool\bin\Release\AdvancedFilterModules.dll .
copy %REPOS%\build-kiptool\bin\Release\AdvancedFilterModulesGUI.dll .
copy %REPOS%\build-imagingsuite\bin\Release\AdvancedFilters.dll .
copy %REPOS%\build-kiptool\bin\Release\BaseModules.dll .
copy %REPOS%\build-kiptool\bin\Release\BaseModulesGUI.dll .
copy %REPOS%\build-kiptool\bin\Release\ClassificationModules.dll .
copy %REPOS%\build-kiptool\bin\Release\ClassificationModulesGUI.dll . 
copy %REPOS%\build-kiptool\bin\Release\ImagingModules.dll .
copy %REPOS%\build-kiptool\bin\Release\ImagingModulesGUI.dll .

copy %REPOS%\imagingsuite\external\lib64\libtiff.dll .
copy %REPOS%\imagingsuite\external\lib64\libjpeg-62.dll .
copy %REPOS%\imagingsuite\external\lib64\zlib1.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3f-3.dll .
copy %REPOS%\imagingsuite\external\lib64\liblapack.dll .
copy %REPOS%\imagingsuite\external\lib64\libgcc_s_seh_64-1.dll .
copy %REPOS%\imagingsuite\external\lib64\libgcc_s_seh-1.dll .
copy %REPOS%\imagingsuite\external\lib64\libgfortran_64-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libquadmath-0.dll .
copy %REPOS%\imagingsuite\external\lib64\libwinpthread-1.dll .
copy %REPOS%\imagingsuite\external\lib64\libblas.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NeXus.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NexusCPP.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5_cpp.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\zlib.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\szip.dll .

@REM copy %REPOS%\imagingsuite\external\lib64\libgcc_s_seh_64-1.dll .
@REM copy %REPOS%\imagingsuite\external\lib64\libgcc_s_seh-1.dll .
@REM copy %REPOS%\imagingsuite\external\lib64\libwinpthread-1.dll .

copy %REPOS%\ExternalDependencies\windows\bin\libopenblas.dll .
copy %REPOS%\ExternalDependencies\windows\bin\libxml2.dll .
copy %REPOS%\ExternalDependencies\windows\bin\cfitsio.dll .

cd %QTBINPATH%

windeployqt %DEST%\KipTool.exe
windeployqt %DEST%\KipToolCLI.exe
copy Qt6PrintSupport.dll %DEST%
copy Qt6Charts.dll %DEST%
copy Qt6OpenGLWidgets.dll %DEST%
copy Qt6OpenGl.dll %DEST%


popd
for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set ldt=%%j
set ldt=%ldt:~0,8%
FOR /F "tokens=*" %%g IN ('git rev-parse --short HEAD') do (set tag=%%g)

if exist "C:\Program Files\7-Zip\7z.exe" (
    "C:\Program Files\7-Zip\7z.exe" a %DEST%\..\KipTool-Windows_build-%tag%-%ldt%.zip %DEST%
) else (
    echo 7zip doesn't exist
)

