#-------------------------------------------------
#
# Project created by QtCreator 2013-12-30T19:34:12
#
#-------------------------------------------------

QT       += core

TARGET = PCAModules
TEMPLATE = lib
CONFIG += c++11

REPOS = $$PWD/../../../../../../..

CONFIG(release, debug|release):    DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

unix:!macx {
    QMAKE_CXXFLAGS += -fopenmp -fPIC -O2
    QMAKE_LFLAGS += -lgomp
    LIBS += -lgomp
}

unix:macx {
    QMAKE_CXXFLAGS += -fPIC -O2
    INCLUDEPATH += /opt/local/include /opt/local/include/libxml2
    QMAKE_LIBDIR += /opt/local/lib
}


DEFINES += PCAMODULES_LIBRARY

SOURCES += ../../src/pcamodules.cpp \
    ../../src/PCAFilterModule.cpp \
    ../../src/stdafx.cpp

HEADERS += \
    ../../src/PCAFilterModule.h \
    ../../src/targetver.h \
    ../../src/stdafx.h \
    ../../src/PCAModules_global.h \
    ../../src/pcamodules.h


win32 {
    INCLUDEPATH  += $$REPOS/imagingsuite/external/include $$REPOS/imagingsuite/external/include/cfitsio $$REPOS/imagingsuite/external/include/libxml2
    QMAKE_LIBDIR += $$_PRO_FILE_PWD_/../../../../../../external/lib64
    LIBS += -llibxml2_dll -llibtiff -lcfitsio
}

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEFEEBACE
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = PCAModules.dll
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

    LIBS += -ltiff -lxml2 -larmadillo
    INCLUDEPATH += /usr/include/libxml2
}

macx: {
INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
DEPENDPATH  += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
LIBS += -L$$REPOS/imagingsuite/external/mac/lib/ -lNeXus.1.0.0 -lNeXusCPP.1.0.0
LIBS += -larmadillo -llapack -lblas
}

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
        QMAKE_LFLAGS += /MACHINE:X64
    }
    INCLUDEPATH +=  $$REPOS/imagingsuite/external/include $$REPOS/imagingsuite/external/include/cfitsio $$REPOS/imagingsuite/external/include/libxml2
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2 /DNOMINMAX
}

CONFIG(release, debug|release): LIBS += -L$$REPOS/lib
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug/

LIBS += -lkipl -lModuleConfig -lProcessFramework

INCLUDEPATH += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include
DEPENDPATH  += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ModuleConfig/include

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH  += $$REPOS/imagingsuite/core/kipl/kipl/include
