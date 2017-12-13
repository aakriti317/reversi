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
    QObject *item=rootobject->findChild<QObject*>("board");
    Board board(item);
    //board.occupy_cell(4,5,"WHITE");
    return app.exec();
}
