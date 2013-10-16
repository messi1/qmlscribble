#include <QGuiApplication>
#include <QQuickView>
#include "drawarea.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<DrawArea>("CustomComponents", 1, 0, "DrawArea");

    QQuickView viewer;
    viewer.setSource(QUrl("qrc:/qml/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.show();

    return app.exec();
}
