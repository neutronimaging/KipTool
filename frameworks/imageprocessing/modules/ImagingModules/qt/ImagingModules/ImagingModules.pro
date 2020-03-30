#-------------------------------------------------
#
# Project created by QtCreator 2013-12-05T10:46:39
#
#-------------------------------------------------

QT       += core

TARGET = ImagingModules
TEMPLATE = lib
CONFIG += c++11

REPOS = $$PWD/../../../../../../..

CONFIG(release, debug|release):    DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

DEFINES += IMAGINGMODULES_LIBRARY

SOURCES +=  ../../src/imagingmodules.cpp \
            ../../src/translateprojectionmodule.cpp \
            ../../src/stripefiltermodule.cpp \
            ../../src/ringcleanmodule.cpp \
            ../../src/morphspotcleanmodule.cpp\
            ../../src/bblognorm.cpp
#            ../../src/referenceimagecorrectionmodule.cpp

HEADERS +=  ../../src/ImagingModules_global.h \
            ../../src/imagingmodules.h\
            ../../src/ImagingModules_global.h \
            ../../src/translateprojectionmodule.h \
            ../../src/stripefiltermodule.h \
            ../../src/ringcleanmodule.h \
            ../../src/morphspotcleanmodule.h \
            ../../src/bblognorm.h
#            ../../src/referenceimagecorrectionmodule.h

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


macx: {
INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
DEPENDPATH  += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
LIBS += -L$$REPOS/imagingsuite/external/mac/lib/ -lNeXus.1.0.0 -lNeXusCPP.1.0.0
}


symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2A168C3
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ImagingModules.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
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
    INCLUDEPATH  += $$REPOS/imagingsuite/external/src/linalg $$REPOS/imagingsuite/external/include $$REPOS/imagingsuite/external/include/cfitsio $$REPOS/imagingsuite/external/include/libxml2
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2 /DNOMINMAX
}



CONFIG(release, debug|release):    LIBS += -L$$REPOS/lib
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug/

LIBS += -lkipl -lModuleConfig -lProcessFramework -lImagingAlgorithms -lReaderConfig


INCLUDEPATH += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/include
DEPENDPATH  += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/include


INCLUDEPATH += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include
DEPENDPATH  += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ModuleConfig/include

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH  += $$REPOS/imagingsuite/core/kipl/kipl/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ReaderConfig
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ReaderConfig
