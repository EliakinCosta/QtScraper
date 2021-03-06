TEMPLATE = lib
TARGET = QtScraper
QT += qml quick xmlpatterns
CONFIG += plugin c++11

LIBS += -ltidy

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.ifba.edu.scraping

# Input
SOURCES += \
        defaultresponseparser.cpp \
        httprequestmodel.cpp \
        iqwebscraperresponseparser.cpp \
        parserprototype.cpp \
        qnamsingleton.cpp \
        qscrapengine.cpp \
        qtscraper_plugin.cpp \
        qwebscraper.cpp \
        qwebscraperaction.cpp \
        qwebscraperresponseparser.cpp \
        qwebscraperstatus.cpp \
        tableresponseparser.cpp

HEADERS += \
        defaultresponseparser.h \
        httprequestmodel.h \
        iqwebscraperresponseparser.h \
        parserprototype.h \
        qnamsingleton.h \
        qscrapengine.h \
        qtscraper_plugin.h \
        qwebscraper.h \
        qwebscraperaction.h \
        qwebscraperresponseparser.h \
        qwebscraperstatus.h \
        tableresponseparser.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
