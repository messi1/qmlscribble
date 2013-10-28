#include <QGuiApplication>
#include <QQuickView>
#include "drawarea.h"
#include "scribblearea.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<DrawArea>("CustomComponents", 1, 0, "DrawArea");
    qmlRegisterType<ScribbleArea>("CustomComponents", 1, 0, "ScribbleArea");

    QQuickView viewer;
    viewer.setSource(QUrl("qrc:/qml/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.show();

    QObject::connect((QObject*)viewer.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    return app.exec();
}
