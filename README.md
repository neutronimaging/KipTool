# KipTool
KipTool is a tool for three analysis of 3D images. The main processing features are 
- Scattering correction of time series.
- Denoising using advanced filtering techniques.

## Build
KipTool requires that imaging suit is built as it depends on many support libraries in the imaging suite.
There are two ways to build KipTool using shell scripts calling QMake and make or by using CMake (in preparation).

### Build using CMake
__Please note:__ this currently only works for arm64.  
Preparations, move outside of the KipTool repository tree then create a build folder. It is assumed that you alread created an install folder when you built the imaging suite
```bash
mkdir build_kiptool
cd build_kiptool
```
Run cmake configuration
```bash
cmake ../KipTool -DCMAKE_INSTALL_PREFIX=../install -DDYNAMIC_LIB=ON -DCMAKE_PREFIX_PATH=<path to your Qt installation>
```
and build 
```bash
cmake --build . --target install
```

Currently, the build doesn't deploy the application. So, you need to run the deployment script separately. 
