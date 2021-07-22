QT += core gui webenginewidgets svg concurrent

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

HEADERS += \
        mainwindow.h \
        globaldefine.h \
        httpd.h \
        httplib.h \
        widget.h \
        webengineview.h

SOURCES += \
        main.cpp \
        httpd.cpp \
        mainwindow.cpp \
        widget.cpp \
        webengineview.cpp

RESOURCES += \
          imgs.qrc

TRANSLATIONS += \
             translations/spark-webapp-runtime_zh_CN.ts
