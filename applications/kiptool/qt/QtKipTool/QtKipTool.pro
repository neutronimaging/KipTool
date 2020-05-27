#-------------------------------------------------
#
# Project created by QtCreator 2013-10-30T07:42:56
#
#-------------------------------------------------

QT       += core widgets charts concurrent # printsupport

TARGET    = KipTool
VERSION   = 2.7.0
TEMPLATE  = app
CONFIG   += c++11

REPOS     = $$PWD/../../../../..

DEFINES  += VERSION=\\\"$$VERSION\\\"

CONFIG(release, debug|release): DESTDIR = $$REPOS/Applications
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/Applications/debug

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target

    unix:macx {
        QMAKE_CXXFLAGS += -fPIC -O2
        INCLUDEPATH += /opt/local/include /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib

    }
    else {
        QMAKE_CXXFLAGS += -fPIC -fopenmp -O2
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
        INCLUDEPATH += /usr/include/libxml2
        INCLUDEPATH += /usr/include/cfitsio
    }

    LIBS += -ltiff -lxml2 -lcfitsio

}

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
        QMAKE_LFLAGS += /MACHINE:X64
    }
    INCLUDEPATH += $$REPOS/imagingsuite/external/src/linalg $$REPOS/imagingsuite/external/include $$REPOS/imagingsuite/external/include/cfitsio $$REPOS/imagingsuite/external/include/libxml2
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2
}

unix:mac {
exists($$REPOS/imagingsuite/external/mac/lib/*NeXus*) {

    message("-lNeXus exists")
    DEFINES *= HAVE_NEXUS

    INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include/ $$REPOS/imagingsuite/external/mac/include/nexus $$REPOS/imagingsuite/external/mac/include/hdf5
    DEPENDPATH += $$REPOS/imagingsuite/external/mac/include/ $$REPOS/imagingsuite/external/mac/include/nexus $$REPOS/imagingsuite/external/mac/include/hdf5
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/mac/lib/

    LIBS += -lNeXus.1.0.0 -lNeXusCPP.1.0.0


}
else {
message("-lNeXus does not exist $$HEADERS")
}

}

ICON = kiptool_icon.icns
RC_ICONS = kiptool_icon.ico

SOURCES +=  ../../src/main.cpp\
            ../../src/kiptoolmainwindow.cpp \
            ../../src/ImageIO.cpp \
            ../../src/confighistorydialog.cpp \
            ../../src/genericconversion.cpp \
            ../../src/Reslicer.cpp \
            ../../src/reslicerdialog.cpp \
            ../../src/mergevolume.cpp \
            ../../src/mergevolumesdialog.cpp \
            ../../src/ImagingToolConfig.cpp \
            ../../src/fileconversiondialog.cpp \
            ../../src/Fits2Tif.cpp \
            ../../src/imagingmoduleconfigurator.cpp \
            ../../src/loadimagedialog.cpp \
            ../../src/processdialog.cpp


HEADERS  += ../../src/kiptoolmainwindow.h \
            ../../src/ImageIO.h \
            ../../src/confighistorydialog.h \
            ../../src/genericconversion.h \
            ../../src/Reslicer.h \
            ../../src/reslicerdialog.h \
            ../../src/mergevolume.h \
            ../../src/mergevolumesdialog.h \
            ../../src/ImagingToolConfig.h \
            ../../src/fileconversiondialog.h \
            ../../src/Fits2Tif.h \
            ../../src/imagingmoduleconfigurator.h \
            ../../src/loadimagedialog.h \
            ../../src/processdialog.h

FORMS    += ../../src/kiptoolmainwindow.ui \
            ../../src/confighistorydialog.ui \
            ../../src/genericconversion.ui \
            ../../src/reslicerdialog.ui \
            ../../src/mergevolumesdialog.ui \
            ../../src/fileconversiondialog.ui \
            ../../src/loadimagedialog.ui \
            ../../src/processdialog.ui

CONFIG(release, debug|release):    LIBS += -L$$REPOS/lib/
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug/

LIBS += -lkipl -lModuleConfig -lProcessFramework -lQtAddons -lQtModuleConfigure -lReaderConfig -lReaderGUI -lImagingAlgorithms

INCLUDEPATH += $$PWD/../../../../frameworks/imageprocessing/ProcessFramework/include
DEPENDPATH  += $$PWD/../../../../frameworks/imageprocessing/ProcessFramework/include/

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtAddons
DEPENDPATH  += $$REPOS/imagingsuite/GUI/qt/QtAddons

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ModuleConfig/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ReaderConfig
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ReaderConfig

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ReaderGUI
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ReaderGUI

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH  += $$REPOS/imagingsuite/core/kipl/kipl/include

INCLUDEPATH += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/include
DEPENDPATH  += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/src

DISTFILES += \
    ../../deploykiptool_mac.sh \
    ../../deploykiptool_ubuntu.sh \
    ../../deploykiptool_win.bat
