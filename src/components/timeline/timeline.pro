# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/timeline
# Target is a library:  

HEADERS += tuptimeline.h \
           tuplayermanager.h \
           tuptlruler.h \
           tupframestable.h \ 
           tuplayerindex.h \
           tuplayercontrols.h
SOURCES += tuptimeline.cpp \
           tuplayermanager.cpp \
           tuptlruler.cpp \
           tupframestable.cpp \
           tuplayerindex.cpp \
           tuplayercontrols.cpp

CONFIG += static warn_on
TEMPLATE = lib
TARGET = timeline

FRAMEWORK_DIR = "../../framework"
include($$FRAMEWORK_DIR/framework.pri)
include(../components_config.pri)
