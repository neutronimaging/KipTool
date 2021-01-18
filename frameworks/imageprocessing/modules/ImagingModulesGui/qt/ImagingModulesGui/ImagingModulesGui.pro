#-------------------------------------------------
#
# Project created by QtCreator 2018-09-13T10:10:46
#
#-------------------------------------------------

QT       += core widgets charts

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = ImagingModulesGUI
TEMPLATE = lib
CONFIG  += c++11

REPOS    = $$PWD/../../../../../../..

CONFIG(release, debug|release): DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

DEFINES += IMAGINGMODULESGUI_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += ../../src/imagingmodulesgui.cpp \
           ../../src/bblognormdlg.cpp \
           ../../src/WaveletRingCleanDlg.cpp \
           ../../src/morphspotcleandlg.cpp

HEADERS  += ../../src/imagingmodulesgui.h \
            ../../src/bblognormdlg.h \
            ../../src/imagingmodulesgui_global.h \
            ../../src/WaveletRingCleanDlg.h \
            ../../src/morphspotcleandlg.h

FORMS    += ../../src/bblognormdlg.ui \
            ../../src/WaveletRingCleanDlg.ui \
            ../../src/morphspotcleandlg.ui

unix:!macx {
    QMAKE_CXXFLAGS += -fopenmp -fPIC -O2
    QMAKE_LFLAGS += -lgomp
    LIBS += -lgomp
    INCLUDEPATH += /usr/include/libxml2
}

unix:macx {
    QMAKE_CXXFLAGS += -fPIC -O2
    INCLUDEPATH += /opt/local/include
    QMAKE_LIBDIR += /opt/local/lib
    INCLUDEPATH += /opt/local/include/libxml2

}


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


win32 {
    contains(QMAKE_HOST.arch, x86_64):{
        QMAKE_LFLAGS += /MACHINE:X64
    }

    INCLUDEPATH += $$REPOS/imagingsuite/external/include
    INCLUDEPATH += $$REPOS/imagingsuite/external/include/cfitsio
    INCLUDEPATH += $$REPOS/imagingsuite/external/include/libxml2

    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2 /DNOMINMAX
}

CONFIG(release, debug|release):    LIBS += -L$$REPOS/lib
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug/

LIBS += -lkipl -lQtModuleConfigure -lImagingAlgorithms  -lQtAddons -lProcessFramework -lModuleConfig -lReaderConfig -lImagingModules


INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure
DEPENDPATH  += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtAddons
DEPENDPATH  += $$REPOS/imagingsuite/GUI/qt/QtAddons

INCLUDEPATH += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/include
DEPENDPATH  += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/include

INCLUDEPATH += $$PWD/../../../../ProcessFramework/include
DEPENDPATH  += $$PWD/../../../../ProcessFramework/include

INCLUDEPATH += $$PWD/../../../ImagingModules/src
DEPENDPATH  += $$PWD/../../../ImagingModules/src

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH  += $$REPOS/imagingsuite/core/kipl/kipl/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ModuleConfig/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ReaderConfig
DEPENDPATH  += $$PWD/../../../../../../core/modules/ReaderConfig

macx: {
INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
DEPENDPATH  += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
LIBS        += -L$$REPOS/imagingsuite/external/mac/lib/ -lNeXus.1.0.0 -lNeXusCPP.1.0.0
}





