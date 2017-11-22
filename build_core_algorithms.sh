#!/bin/bash
REPOSPATH=/Users/kaestner/git/imagingsuite

DEST=/Users/kaestner/git/builds

mkdir -p $DEST/build-imagingalgorithms
cd $DEST/build-imagingalgorithms

$QTBINPATH/qmake -makefile -r -spec macx-clang CONFIG+=x86_64 -o Makefile ../../imagingsuite/core/algorithms/ImagingAlgorithms/qt/ImagingAlgorithms/ImagingAlgorithms.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile

mkdir -p $DEST/build-imagingqaalgorithms
cd $DEST/build-imagingqaalgorithms

$QTBINPATH/qmake -makefile -r -spec macx-clang CONFIG+=x86_64 -o Makefile ../../imagingsuite/core/algorithms/ImagingQAAlgorithms/ImagingQAAlgorithms.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile


if [ -e "$REPOSPATH/core/algorithms/UnitTests" ]
then
    echo "Build tests"

    for f in `ls $REPOSPATH/core/algorithms/UnitTests`
    do
        echo "$REPOSPATH/core/algorithms/UnitTests/$f/$f.pro"
        if [ -e "$REPOSPATH/core/algorithms/UnitTests/$f/$f.pro" ]
        then
            mkdir -p $DEST/build-$f
            cd $DEST/build-$f

            $QTBINPATH/qmake -makefile -o Makefile ../../imagingsuite/core/algorithms/UnitTests/$f/$f.pro
            make -f Makefile clean
            make -f Makefile mocables all
            make -f Makefile
        fi

    done

    echo "Tests built"
fi
