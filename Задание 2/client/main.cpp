#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QTextStream>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    quint16 port = 9001;
    if (argc >= 2)
        port = QString(argv[1]).toUShort();

    Client client;
    client.connectToServer("127.0.0.1", port);

    QTextStream input(stdin);
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&client, &input]() {
        if (input.atEnd()) return;
        QString line = input.readLine();
        if (!line.isEmpty()) {
            client.sendGuess(line);
        }
    });
    timer.start(200);

    return a.exec();
}
