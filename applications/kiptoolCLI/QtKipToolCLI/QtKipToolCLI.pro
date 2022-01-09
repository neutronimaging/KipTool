#-------------------------------------------------
#
# Project created by QtCreator 2018-12-10T10:46:31
#
#-------------------------------------------------

CONFIG += c++11 console
CONFIG -=app_bundle
CONFIG += no_batch

TARGET = KipToolCLI
#VERSION = 2.7.0
#TEMPLATE = app

REPOS = $$PWD/../../../..

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += QT_DEPRECATED_WARNINGS


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
    }

    LIBS += -ltiff -lxml2 -lcfitsio

}

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
        QMAKE_LFLAGS += /MACHINE:X64
    }

    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/libxml2
    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/cfitsio
    QMAKE_LIBDIR += $$REPOS/ExternalDependencies/windows/lib

    INCLUDEPATH += $$REPOS/imagingsuite/external/src/linalg $$REPOS/imagingsuite/external/include
    LIBS += -L$$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2 -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2
}



unix:mac {
exists($$PWD/../../../../external/mac/lib/*NeXus*) {

    message("-lNeXus exists")
    DEFINES *= HAVE_NEXUS

    INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include/ $$REPOS/imagingsuite/external/mac/include/nexus $$REPOS/imagingsuite/external/mac/include/hdf5
    DEPENDPATH += $$REPOS/imagingsuite/external/mac/include/ $$REPOS/imagingsuite/mac/include/nexus $$$REPOS/imagingsuite/external/mac/include/hdf5
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/mac/lib/

    LIBS += -lNeXus.1.0.0 -lNeXusCPP.1.0.0


}
else {
message("-lNeXus does not exist $$HEADERS")
}

}

#ICON = kiptool_icon.icns
#RC_ICONS = kiptool_icon.ico

SOURCES +=  main.cpp \
            qtkiptoolcli.cpp \
            ../../kiptool/src/ImageIO.cpp \
            ../../kiptool/src/stdafx.cpp
#            ImageIO.cpp
#            stdafx.cpp \
#            ImagingToolConfig.cpp \
#            genericconversion.cpp \
#            Reslicer.cpp \
#            mergevolume.cpp \
#            $$PWD/../../../kiptool/src/Fits2Tif.cpp \
#            ../../../kiptool/src/ImagingToolConfig.cpp
#            Fits2Tif.cpp \




HEADERS  += qtkiptoolcli.h \
            ../../kiptool/src/ImageIO.h \
            ../../kiptool/src/stdafx.h
#            ImageIO.h
#            stdafx.h \
#            targetver.h \
#            ImagingToolConfig.h \
#            genericconversion.h \
#            Reslicer.h \
#            mergevolume.h \
#            $$PWD/../../../kiptool/src/Fits2Tif.h  \
#            ../../../kiptool/src/stdafx.h \
#            ../../../kiptool/src/targetver.h \
#            ../../../kiptool/src/ImagingToolConfig.h
#            Fits2Tif.h  \


INCLUDEPATH += $$PWD/../../../frameworks/imageprocessing/ProcessFramework/include
DEPENDPATH  += $$PWD/../../../frameworks/imageprocessing/ProcessFramework/include/

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ModuleConfig/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ReaderConfig
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ReaderConfig

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH  += $$REPOS/imagingsuite/core/kipl/kipl/include

INCLUDEPATH += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/include
DEPENDPATH  += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/src

LIBS += -L$$REPOS/lib/
LIBS += -lkipl -lModuleConfig -lProcessFramework -lReaderConfig  -lImagingAlgorithms


