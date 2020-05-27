#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T07:19:32
#
#-------------------------------------------------

QT       -= gui

TARGET = ProcessFramework
TEMPLATE = lib
CONFIG += c++11

REPOS = $$PWD/../../../../../..
CONFIG(release, debug|release): DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

message("ProcFramework DESTDIR $$DESTDIR")

DEFINES += PROCESSFRAMEWORK_LIBRARY

SOURCES += \
    ../../src/stdafx.cpp \
    ../../src/KiplProcessModuleBase.cpp \
    ../../src/KiplProcessConfig.cpp \
    ../../src/KiplModuleItem.cpp \
    ../../src/KiplFrameworkException.cpp \
    ../../src/KiplFactory.cpp \
    ../../src/KiplEngine.cpp \
    ../../src/dllmain.cpp

HEADERS += \
    ../../include/KiplProcessModuleBase.h \
    ../../include/KiplProcessConfig.h \
    ../../include/KiplModuleItem.h \
    ../../include/KiplFrameworkException.h \
    ../../include/KiplFactory.h \
    ../../include/KiplEngine.h \
    ../../src/targetver.h \
    ../../src/stdafx.h \
    ../../include/ProcessFramework_global.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target

    unix:macx {
        QMAKE_CXXFLAGS += -fPIC -O2
        INCLUDEPATH += /opt/local/include
        INCLUDEPATH += /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib
 #       QMAKE_INFO_PLIST = Info.plist
 #       ICON = muhrec3.icns
    }
    else {
        INCLUDEPATH += /usr/include/cfitsio
        QMAKE_CXXFLAGS += -fPIC -fopenmp -O2
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
    }

    LIBS += -ltiff -lxml2
    INCLUDEPATH += /usr/include/libxml2
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

unix:!mac {
    exists(/usr/lib/*NeXus*) {
        message("-lNeXus exists")
        DEFINES *= HAVE_NEXUS
        LIBS += -lNeXus -lNeXusCPP
    }
    else {
        message("-lNeXus does not exists $$LIBS")
    }
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
message("-lNeXus does not exists $$HEADERS")
}

}

CONFIG(release, debug|release): LIBS += -L$$REPOS/lib/
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug/

LIBS+= -lkipl -lModuleConfig

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH += $$REPOS/imagingsuite/core/kipl/kipl/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
