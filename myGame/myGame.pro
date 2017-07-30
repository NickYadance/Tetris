
QT += core gui

QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


HEADERS += \
    onebox.h \
    boxgroup.h \
    myview.h

SOURCES += \
    onebox.cpp \
    boxgroup.cpp \
    myview.cpp \
    main.cpp

RESOURCES += \
    myimages.qrc

