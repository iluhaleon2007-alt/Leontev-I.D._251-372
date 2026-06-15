#include "client.h"
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &Client::onError);
}

void Client::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "Connecting to" << host << ":" << port;
    m_socket->connectToHost(host, port);
}

void Client::sendGuess(const QString &guess)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(guess.toUtf8() + "\n");
        m_socket->flush();
        qDebug() << "Sent:" << guess;
    }
}

void Client::onConnected()
{
    qDebug() << "Connected to server";
}

void Client::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    QString message = QString::fromUtf8(data).trimmed();
    qDebug() << "Server:" << message;
}

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server";
}

void Client::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    qDebug() << "Error:" << m_socket->errorString();
}
