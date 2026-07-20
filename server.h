#pragma once

#include <QMainWindow>

#include <QObject>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTime>
#include "functionsoftransformingcpp.h"
#include <QtNetwork>


class ServerWindow;

class TheodoliteServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TheodoliteServer(ServerWindow* window = nullptr, QTcpServer *parent = nullptr);
    ~TheodoliteServer();
    enum class MyData : quint16
    {
        VECHS,
        CONTROL,
        CMDFLIP,
    };
    QString MakeString(QDataStream& into_string);
signals:
    void DataReciever(QByteArray data);
    void EmitMessage(const QString& message); //Сигнал для передачи сообщения
public slots:

    //обработчик входящего подключения:
    void incomingConnection(qintptr socketDescriptor);
    void Read();
    void stateChanged(QAbstractSocket::SocketState stat); // обработчик изменения состояния вещающего сокета (клиента)
    void sockDisc();
    void readAndSendPendingDatagrams();
private:
    ServerWindow* srw;
    QUdpSocket *udp_srever_socket;
    QTcpSocket *ARMsocket;
    QVector< std::variant<Vechs, Control> > qvect;
    TheodoliteInstance MyStruct_TheodoliteInstance;
    quint16 m_nNextBlockSize;
    QString udp_req;
};

