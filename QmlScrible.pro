QT += qml quick

CONFIG+=qml_debug

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
