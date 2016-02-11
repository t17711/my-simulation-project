#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtWidgets/QStandardItemModel>

#include "citymodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView viewer;
    CityModel model;

    viewer.engine()->rootContext()->setContextProperty("_model", &model);
    viewer.setSource(QUrl("qrc:///main.qml"));
    viewer.show();

    return app.exec();
}
