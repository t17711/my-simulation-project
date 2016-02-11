#include <QQmlEngine>
#include <QQuickView>

#include <QApplication>


int main(int argc, char ** argv)
{

    QApplication app(argc, argv);

    QQuickView *view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("test.qml"));
    view->show();

    QQmlEngine* engine = view->engine();
    QObject::connect(engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    return app.exec();
}

