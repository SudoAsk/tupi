# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/libbase
# Target is a library: tupibase

INSTALLS += headers target 
target.path = /lib/ 

headers.target = .
headers.commands = cp *.h $(INSTALL_ROOT)/include/tupibase
headers.path = /include/tupibase

HEADERS += tupexportinterface.h \
           tupexportpluginobject.h \
           tupfilterinterface.h \
           tupmodulewidgetbase.h \
           tuppluginmanager.h \
           tuptoolinterface.h \
           tuptoolplugin.h  \
           tuppaintareabase.h \
           tuppaintarearotator.h \
           tupgraphicsscene.h \
           tupguideline.h \
           tupanimationrenderer.h
SOURCES += tupexportpluginobject.cpp \
           tupmodulewidgetbase.cpp \
           tuppluginmanager.cpp \
           tuptoolplugin.cpp  \
           tuppaintareabase.cpp \
           tuppaintarearotator.cpp \
           tupgraphicsscene.cpp \
           tupguideline.cpp \
           tupanimationrenderer.cpp

CONFIG += dll warn_on
TEMPLATE = lib
TARGET = tupibase

FRAMEWORK_DIR = "../framework"
include($$FRAMEWORK_DIR/framework.pri)
QUAZIP_DIR = ../../3rdparty/quazip/
include($$QUAZIP_DIR/quazip.pri)
LIBTUPI_DIR = ../../src/libtupi
include($$LIBTUPI_DIR/libtupi.pri)
STORE_DIR = ../../src/store
include($$STORE_DIR/store.pri)

include(../../tupiglobal.pri)
