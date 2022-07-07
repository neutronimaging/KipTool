#!/bin/bash
#CPCMD="ln -s "
CPCMD="cp "
REPOSPATH=$WORKSPACE
QT_PATH=$QTBINPATH/..
#declare -i QtV=${QT_PATH:end-11:end-8}
QtV=${QT_PATH:end-17:end-14}
QtVmain=${QT_PATH:end-17:end-16}

echo $QtV
echo $QtVmain
echo $QT_PATH


DEST="$REPOSPATH/deploy/kiptool"

mkdir --parent $DEST
mkdir --parent $DEST/bin
mkdir --parent $DEST/bin/platforms
mkdir --parent $DEST/Frameworks

pushd .
cd $DEST/Frameworks
rm -f *.so.1.0.0

`$CPCMD $REPOSPATH/install/lib/libQtAddons.so .`
`$CPCMD $REPOSPATH/install/lib/libQtModuleConfigure.so .`

`$CPCMD $REPOSPATH/install/lib/libImagingAlgorithms.so .`
`$CPCMD $REPOSPATH/install/lib/libAdvancedFilters.so .`
`$CPCMD $REPOSPATH/install/lib/libkipl.so .`
`$CPCMD $REPOSPATH/install/lib/libModuleConfig.so .`
`$CPCMD $REPOSPATH/install/lib/libProcessFramework.so .`

`$CPCMD $REPOSPATH/install/lib/libReaderConfig.so .`
`$CPCMD $REPOSPATH/install/lib/libReaderGUI.so .`

# Module libs
`$CPCMD $REPOSPATH/install/lib/libAdvancedFilterModules.so .`
`$CPCMD $REPOSPATH/install/lib/libAdvancedFilterModulesGUI.so .`
`$CPCMD $REPOSPATH/install/lib/libBaseModules.so .`
`$CPCMD $REPOSPATH/install/lib/libBaseModulesGUI.so  .`
#`$CPCMD $REPOSPATH/lib/libPorespaceModules.so.1.0.0 .`
`$CPCMD $REPOSPATH/install/lib/libClassificationModules.so .`
`$CPCMD $REPOSPATH/install/lib/libClassificationModulesGUI.so .`
`$CPCMD $REPOSPATH/install/lib/libImagingModules.so .`
#`$CPCMD $REPOSPATH/lib/libPCAModules.so.1.0.0 .`
#`$CPCMD $REPOSPATH/lib/libStatisticsModules.so.1.0.0 .`
`$CPCMD $REPOSPATH/install/lib/libImagingModulesGUI.so .`

#qt stuff
# `$CPCMD $QT_PATH/lib/libQt5Core.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libQt5Gui.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libQt5Widgets.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libQt5DBus.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libQt5PrintSupport.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libQt5XcbQpa.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libQt5DBus.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libQt5Charts.so.5.$QtV .`
# `$CPCMD $QT_PATH/lib/libicui18n.so.56.1 .`
# `$CPCMD $QT_PATH/lib/libicudata.so.56.1 .`
# `$CPCMD $QT_PATH/lib/libicuuc.so.56.1 .`

`$CPCMD $QT_PATH/lib/libQt6Core.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6Gui.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6Widgets.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6DBus.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6PrintSupport.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6OpenGLWidgets.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6OpenGL.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6XcbQpa.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6DBus.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libQt6Charts.so.6.$QtV .`
`$CPCMD $QT_PATH/lib/libicui18n.so.56.1 .`
`$CPCMD $QT_PATH/lib/libicudata.so.56.1 .`
`$CPCMD $QT_PATH/lib/libicuuc.so.56.1 .`


# rm -f *.so
# rm -f *.so.1
# rm -f *.so.1.0

for f in `ls *.so.1.0.0`; do
	bn=`basename $f .so.1.0.0`
	echo $bn
	ln -s $f $bn.so.1.0
	ln -s $bn.so.1.0 $bn.so.1
	ln -s $bn.so.1 $bn.so
done

rm -f *.so.6.$QtVmain
rm -f *.so.6
for f in `ls *.so.6.$QtV`; do
	bn=`basename $f .so.6.$QtV`
	echo $bn
	ln -s $f $bn.so.6.$QtVmain
        ln -s $bn.so.6.$QtVmain $bn.so.6
	ln -s $bn.so.6 $bn.so
done

rm -f *.so.56
for f in `ls *.so.56.1`; do
	bn=`basename $f .so.56.1`
	echo $bn
	ln -s $bn.so.56.1 $bn.so.56
	ln -s $bn.so.56 $bn.so
done


cd $DEST/bin/platforms
`$CPCMD $QT_PATH/plugins/platforms/libqxcb.so .`


cd $DEST/bin
`$CPCMD $REPOSPATH/install/applications/KipTool .`
cd ..
`$CPCMD $REPOSPATH/KipTool/applications/kiptool/scripts/kiptool .`
chmod +x kiptool

fname=kiptool_`uname -s`_`uname -m`_`date +%Y%m%d`.tar.bz2

echo $fname

cd $DEST
cd ..
tar -jcvhf $fname 'kiptool'


popd


