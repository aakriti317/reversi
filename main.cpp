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
    Board board;
    QObject::connect(item, SIGNAL(piece_added_signal(qint32,qint32,QString)),
                     &board, SLOT(piece_added_slot(qint32,qint32,QString)));
    return app.exec();
}
