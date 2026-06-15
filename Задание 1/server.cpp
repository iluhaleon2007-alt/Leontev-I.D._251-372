#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);

    if (!m_server->listen(QHostAddress::Any, 9000)) {
        qDebug() << "Server could not start";
    } else {
        qDebug() << "Server started on port 9000";
    }
}

void Server::onNewConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    qDebug() << "New connection";
}

void Server::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    qDebug() << "Received:" << data;
    socket->write("Hello from server!\n");
    socket->flush();
}

void Server::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        socket->deleteLater();
        qDebug() << "Disconnected";
    }
}
