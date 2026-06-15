#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QMap>
#include <ctime>
#include <cstdlib>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    void sendToClient(QTcpSocket *socket, const QString &str);
    void broadcast(const QString &message);
    void broadcastClientCount();
    void startGame();
    void nextTurn();

    QTcpServer *mTcpServer;
    QVector<QTcpSocket*> mClients;
    QMap<QTcpSocket*, QByteArray> mBuffers;
    int m_maxClients = 5;
    int m_targetNumber;
    int m_currentTurn = 0;
    bool m_gameStarted = false;
};

#endif // MYTCPSERVER_H
