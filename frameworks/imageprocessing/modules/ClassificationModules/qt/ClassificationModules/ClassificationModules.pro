#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T18:22:19
#
#-------------------------------------------------

QT       += core

TARGET   = ClassificationModules
TEMPLATE = lib
CONFIG  += c++11

REPOS    = $$PWD/../../../../../../..

CONFIG(release, debug|release):    DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

unix:!symbian {
    QMAKE_CXXFLAGS += -fPIC -O2

    unix:!macx {
        QMAKE_CXXFLAGS += -fopenmp
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
    }

    unix:macx {
        INCLUDEPATH += /opt/local/include /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib
    }

    LIBS += -ltiff
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

DEFINES += CLASSIFICATIONMODULES_LIBRARY

SOURCES += \
    ../../src/RemoveBackground.cpp \
    # ../../src/KernelFuzzyCMeans.cpp \
    #../../src/FuzzyCMeans.cpp \
    ../../src/DoubleThreshold.cpp \
    ../../src/ClassificationModules.cpp \
    ../../src/BasicThreshold.cpp

HEADERS += \
    ../../src/RemoveBackground.h \
   # ../../src/KernelFuzzyCMeans.h \
   # ../../src/FuzzyCMeans.h \
    ../../src/DoubleThreshold.h \
    ../../src/ClassificationModules.h \
    ../../src/BasicThreshold.h \
    ../../src/ClassificationModules_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEA26CA15
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ClassificationModules.dll
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

    LIBS += -ltiff -lxml2
    INCLUDEPATH += /usr/include/libxml2
}

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
        QMAKE_LFLAGS += /MACHINE:X64
    }

    INCLUDEPATH  += $$REPOS/imagingsuite/external/include $$REPOS/imagingsuite/external/include/cfitsio $$REPOS/imagingsuite/external/include/libxml2
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2 /DNOMINMAX
}


macx: {
INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
DEPENDPATH  += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
LIBS        += -L$$REPOS/imagingsuite/external/mac/lib/ -lNeXus.1.0.0 -lNeXusCPP.1.0.0
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


