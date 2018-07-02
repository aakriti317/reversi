#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <board.h>


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    QObject *rootobject = engine.rootObjects().first();
    QObject *bobj=rootobject->findChild<QObject*>("board");
    QObject *cobj=rootobject->findChild<QObject*>("controls");
    QObject *hobj=rootobject->findChild<QObject*>("home");

    Board board(rootobject,bobj,cobj,hobj);
    return app.exec();
}
