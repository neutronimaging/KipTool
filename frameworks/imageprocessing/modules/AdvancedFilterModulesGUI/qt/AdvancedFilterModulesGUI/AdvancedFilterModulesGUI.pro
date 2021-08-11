#-------------------------------------------------
#
# Project created by QtCreator 2018-09-21T07:14:35
#
#-------------------------------------------------
QT       += core widgets charts

TARGET = AdvancedFilterModulesGUI
TEMPLATE = lib
CONFIG += c++11

REPOS = $$PWD/../../../../../../..

CONFIG(release, debug|release):    DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

DEFINES += ADVANCEDFILTERMODULESGUI_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

INCLUDEPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
DEPENDPATH += $$REPOS/imagingsuite/external/mac/include $$REPOS/imagingsuite/external/mac/include/hdf5 $$REPOS/imagingsuite/external/mac/include/nexus
LIBS += -L$$REPOS/imagingsuite/external/mac/lib/ -lNeXus.1.0.0 -lNeXusCPP.1.0.0
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

SOURCES += \
    ../../src/advancedfiltermodulesgui.cpp \
    ../../src/issfilterdlg.cpp \
    ../../src/nonlindiffusiondlg.cpp

HEADERS += \
    ../../src/advancedfiltermodulesgui.h \
    ../../src/advancedfiltermodulesgui_global.h \
    ../../src/issfilterdlg.h \
    ../../src/nonlindiffusiondlg.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ../../src/issfilterdlg.ui \
    ../../src/nonlindiffusiondlg.ui

CONFIG(release, debug|release):    LIBS += -L$$REPOS/lib
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug/
LIBS += -lkipl -lModuleConfig -lProcessFramework -lQtModuleConfigure -lQtAddons -lAdvancedFilterModules

INCLUDEPATH += $$PWD/../../../../../../frameworks/imageprocessing/modules/AdvancedFilterModules/src
DEPENDPATH  += $$PWD/../../../../../../frameworks/imageprocessing/modules/AdvancedFilterModules/src

INCLUDEPATH += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include
DEPENDPATH  += $$PWD/../../../../../../frameworks/imageprocessing/ProcessFramework/include


INCLUDEPATH += $$REPOS/imagingsuite/core/algorithms/AdvancedFilters
DEPENDPATH  += $$REPOS/imagingsuite/core/algorithms/AdvancedFilters

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH  += $$REPOS/imagingsuite/core/modules/ModuleConfig/include

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure
DEPENDPATH  += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtAddons
DEPENDPATH  += $$REPOS/imagingsuite/GUI/qt/QtAddons

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH  += $$REPOS/imagingsuite/core/kipl/kipl/include
