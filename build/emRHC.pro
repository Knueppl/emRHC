HEADERS = ../src/Client.h \
          ../src/ConnectDialog.h \
          ../src/MainWindow.h \

SOURCES = ../src/Client.cpp \
          ../src/ConnectDialog.cpp \
          ../src/MainWindow.cpp \
          ../src/main.cpp

FORMS = ../forms/Client.ui \
        ../forms/ConnectDialog.ui \
        ../forms/MainWindow.ui

QT += network
QT += gui
QT += xml
