QT += qml quick

bla.files = qml/QmlScribble
bla.path = qml

INSTALLS = bla

SOURCES += main.cpp \
    drawarea.cpp

HEADERS += \
    drawarea.h

OTHER_FILES += \
    qml/main.qml \
    qml/Cell.qml \
    qml/Button.qml \
    README.md

RESOURCES += \
    resources.qrc
