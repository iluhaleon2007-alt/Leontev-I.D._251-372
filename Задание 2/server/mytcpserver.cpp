#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>

MyTcpServer::~MyTcpServer()
{
    for (QTcpSocket *s : mClients) {
        s->close();
    }
    mTcpServer->close();
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    qsrand(time(0));
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection()
{
    while (mTcpServer->hasPendingConnections()) {
        QTcpSocket *mTcpSocket = mTcpServer->nextPendingConnection();

        if (mClients.size() >= m_maxClients) {
            mTcpSocket->write("Server is busy. Try again later.\r\n");
            mTcpSocket->flush();
            mTcpSocket->disconnectFromHost();
            qDebug() << "Rejected client (server full)";
            return;
        }

        mClients.append(mTcpSocket);
        connect(mTcpSocket, &QTcpSocket::readyRead,
                this, &MyTcpServer::slotServerRead);
        connect(mTcpSocket, &QTcpSocket::disconnected,
                this, &MyTcpServer::slotClientDisconnected);

        qDebug() << "Client connected. Total:" << mClients.size();

        mTcpSocket->write("Hello! Welcome! You are connected to the server.\r\n");
        mTcpSocket->flush();

        broadcastClientCount();

        if (mClients.size() == m_maxClients && !m_gameStarted) {
            startGame();
        }
    }
}

void MyTcpServer::slotServerRead()
{
    QTcpSocket *mTcpSocket = qobject_cast<QTcpSocket*>(sender());
    if (!mTcpSocket) return;

    QString res = "";
    while (mTcpSocket->bytesAvailable() > 0) {
        QByteArray array = mTcpSocket->readAll();
        res.append(array);
    }
    res = res.trimmed();
    qDebug() << "Received:" << res;

    if (!m_gameStarted) {
        mTcpSocket->write("Waiting for all clients...\r\n");
        mTcpSocket->flush();
        return;
    }

    bool ok;
    int guess = res.toInt(&ok);
    if (!ok) {
        mTcpSocket->write("Enter a number!\r\n");
        mTcpSocket->flush();
        return;
    }

    if (mTcpSocket != mClients[m_currentTurn]) {
        mTcpSocket->write("Not your turn!\r\n");
        mTcpSocket->flush();
        return;
    }

    qDebug() << "Client guessed:" << guess;

    if (guess == m_targetNumber) {
        QByteArray winMsg = "Guessed! Number: " + QByteArray::number(m_targetNumber) + "\r\n";
        broadcast(winMsg);
        QByteArray endMsg = "Game over. All connections closed.\r\n";
        broadcast(endMsg);
        qDebug() << "Number guessed! Game over.";

        for (QTcpSocket *s : mClients) {
            s->disconnectFromHost();
        }
        mClients.clear();
        m_gameStarted = false;
    } else if (guess < m_targetNumber) {
        mTcpSocket->write("Higher!\r\n");
        mTcpSocket->flush();
    } else {
        mTcpSocket->write("Lower!\r\n");
        mTcpSocket->flush();
    }

    nextTurn();
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket *mTcpSocket = qobject_cast<QTcpSocket*>(sender());
    if (!mTcpSocket) return;

    int index = mClients.indexOf(mTcpSocket);
    qDebug() << "Client disconnected. Index:" << index;

    mClients.removeOne(mTcpSocket);
    mTcpSocket->close();

    broadcastClientCount();

    if (mClients.isEmpty() && m_gameStarted) {
        m_gameStarted = false;
        qDebug() << "All clients disconnected. Game reset.";
    }
}

void MyTcpServer::sendToClient(QTcpSocket *socket, const QString &str)
{
    socket->write(str.toUtf8());
    socket->flush();
}

void MyTcpServer::broadcast(const QString &message)
{
    QByteArray msg = message.toUtf8();
    for (QTcpSocket *socket : mClients) {
        socket->write(msg);
        socket->flush();
    }
}

void MyTcpServer::broadcastClientCount()
{
    QByteArray msg = "Connected clients: " + QByteArray::number(mClients.size()) +
                     " of " + QByteArray::number(m_maxClients) + "\r\n";
    broadcast(msg);
}

void MyTcpServer::startGame()
{
    m_gameStarted = true;
    m_targetNumber = (qrand() % 100) + 1;
    m_currentTurn = 0;

    qDebug() << "Game started! Target number:" << m_targetNumber;
    broadcast("All clients connected! Game starts!\r\n");
    broadcast("Number from 1 to 100 is guessed. Take turns!\r\n");

    if (!mClients.isEmpty()) {
        mClients[0]->write("Your turn! Enter a number:\r\n");
        mClients[0]->flush();
    }
}

void MyTcpServer::nextTurn()
{
    if (mClients.isEmpty()) return;

    m_currentTurn = (m_currentTurn + 1) % mClients.size();
    mClients[m_currentTurn]->write("Your turn! Enter a number:\r\n");
    mClients[m_currentTurn]->flush();
}
