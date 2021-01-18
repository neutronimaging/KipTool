#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T07:55:35
#
#-------------------------------------------------
QT       += core

TARGET = AdvancedFilterModules
TEMPLATE = lib
CONFIG += c++11
REPOS = $$PWD/../../../../../../..

CONFIG(release, debug|release):    DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

DEFINES += ADVANCEDFILTERMODULES_LIBRARY

SOURCES += \
    ../../src/ISSfilterModule.cpp \
    ../../src/AdvancedFilterModules.cpp \
    ../../src/NonLinDiffusion.cpp \
    ../../src/nonlocalmeansmodule.cpp

HEADERS += \
    ../../src/ISSfilterModule.h \
    ../../src/AdvancedFilterModules.h \
    ../../src/NonLinDiffusion.h \
    ../../src/AdvancedFilterModules_global.h \
    ../../src/nonlocalmeansmodule.h

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

    LIBS += -ltiff -lxml2

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


macx: {
INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
DEPENDPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
LIBS += -L$$REPOS/imagingsuite/external/mac/lib/ -lNeXus.1.0.0 -lNeXusCPP.1.0.0
}

CONFIG(release, debug|release):    LIBS += -L$$REPOS/lib
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug/

LIBS += -lkipl -lModuleConfig -lProcessFramework

INCLUDEPATH += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include
DEPENDPATH  += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ModuleConfig/include

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH  += $$REPOS/imagingsuite/core/kipl/kipl/include
