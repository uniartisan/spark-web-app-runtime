QT += core gui webengine webenginewidgets svg concurrent

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

SOURCES += main.cpp\
        httpd.cpp \
        mainwindow.cpp \
        webenginepage.cpp \
        widget.cpp

HEADERS  += mainwindow.h \
         globaldefine.h \
         httpd.h \
         httplib.h \
         webenginepage.h \
         widget.h

FORMS += widget.ui

RESOURCES += imgs.qrc

TRANSLATIONS += \
    translations/spark-webapp-runtime_zh_CN.ts


