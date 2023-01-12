set REPOS=%WORKSPACE%
set DEST=%WORKSPACE%\deployed\kiptool

IF NOT EXIST %DEST% mkdir %DEST%
pushd .
cd %DEST%

copy %REPOS%\install\applications\KipTool.exe .
@REM copy %REPOS%\Applications\KipToolCLI.exe .
copy %REPOS%\install\lib\ImagingAlgorithms.dll .
copy %REPOS%\install\lib\ModuleConfig.dll .
copy %REPOS%\install\lib\ReaderConfig.dll .
copy %REPOS%\install\lib\ReaderGUI.dll .
copy %REPOS%\install\lib\QtAddons.dll .
copy %REPOS%\install\lib\QtModuleConfigure.dll .
copy %REPOS%\install\lib\kipl.dll .
copy %REPOS%\install\lib\ProcessFramework.dll .
copy %REPOS%\install\lib\AdvancedFilterModules.dll .
copy %REPOS%\install\lib\AdvancedFilterModulesGUI.dll .
copy %REPOS%\install\lib\AdvancedFilters.dll .
copy %REPOS%\install\lib\BaseModules.dll .
copy %REPOS%\install\lib\BaseModulesGUI.dll .
copy %REPOS%\install\lib\ClassificationModules.dll .
copy %REPOS%\install\lib\ClassificationModulesGUI.dll . 
copy %REPOS%\install\lib\ImagingModules.dll .
copy %REPOS%\install\lib\ImagingModulesGUI.dll .

copy %REPOS%\\install\lib\libtiff.dll .
copy %REPOS%\install\lib\libjpeg-62.dll .
copy %REPOS%\install\lib\zlib1.dll .
copy %REPOS%\install\lib\libfftw3-3.dll .
copy %REPOS%\install\lib\libfftw3f-3.dll .
copy %REPOS%\install\lib\liblapack.dll .
copy %REPOS%\install\lib\libgcc_s_seh_64-1.dll .
copy %REPOS%\install\lib\libgcc_s_seh-1.dll .
copy %REPOS%\install\lib\libgfortran_64-3.dll .
copy %REPOS%\install\lib\libquadmath-0.dll .
copy %REPOS%\install\lib\libwinpthread-1.dll .
copy %REPOS%\install\lib\libblas.dll .
copy %REPOS%\install\lib\NeXus.dll .
copy %REPOS%\install\lib\NexusCPP.dll .
copy %REPOS%\install\lib\hdf5.dll .
copy %REPOS%\install\lib\hdf5_cpp.dll .
copy %REPOS%\install\lib\zlib.dll .
copy %REPOS%\install\lib\szip.dll .

@REM copy %REPOS%\install\lib\libgcc_s_seh_64-1.dll .
@REM copy %REPOS%\install\lib\libgcc_s_seh-1.dll .
@REM copy %REPOS%\install\lib\libwinpthread-1.dll .

@REM copy %REPOS%\install\lib\libopenblas.dll .
copy %REPOS%\install\lib\libxml2.dll .
copy %REPOS%\install\lib\cfitsio.dll .

mkdir resources
copy %REPOS%\KipTool\applications\kiptool\resources resources
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

