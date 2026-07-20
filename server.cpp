#include "server.h"



TheodoliteServer::~TheodoliteServer()
{
    delete ARMsocket;
}

TheodoliteServer::TheodoliteServer(ServerWindow* window, QTcpServer *parent) : QTcpServer(parent), srw(window),ARMsocket(nullptr), m_nNextBlockSize(0),udp_req("")
{
    //QTcpServer * server = new QTcpServer(this);
    //QTcpSocket * socket = new QTcpSocket();
    qDebug() << "server listen = " << this->listen(QHostAddress::Any, 8080);
    udp_srever_socket = new QUdpSocket(this);
    udp_srever_socket->bind(QHostAddress::LocalHost, 7000);
    connect(udp_srever_socket,SIGNAL(readyRead()),this,SLOT(readAndSendPendingDatagrams()));

    //QHostAddress::Any - прослушивает все порты на устройстве; 6666 - порт
    //connect(server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    //Замечаем что на сервер пришло новое соединение (сигнал), и передаем его в обработчик SLOT - связываем сигнал с обработчиком
}

void TheodoliteServer::incomingConnection(qintptr socketDescriptor) // обработчик подключений
{
    QTcpSocket * socket = new QTcpSocket();
    qDebug() << "Hello from server";
    if (socket->setSocketDescriptor(socketDescriptor))
    {
          //socket = server->nextPendingConnection(); // получаем сокет нового входящего подключения
        qDebug() << "Hello from 2 server";
    }
    else
    {
        delete socket;
        return;
    }
    //connect(server, SIGNAL(newConnection()), this, SLOT(incomingConnection(socketDescriptor)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState))); // делаем обработчик изменения статуса сокета
    connect(socket, SIGNAL(readyRead()), this, SLOT(Read())); //Подключаем сообщения от сокета на наш обработчик
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    //у метода connect 1-й аргумент - объект класса QObject
    if (!ARMsocket) //Если нет ARMSocket-а - подключаем его и делаем вещающим ()
    {
        socket->write("client"); // записываем внутрь буфера вещающего сокета строку
        ARMsocket = socket;
        qDebug() << "this first client";
    }
    else //Если подключение второе - убиваем сокет и подключаем его заново
    {
        ARMsocket->disconnectFromHost();
        ARMsocket->deleteLater();
        socket->write("client");
        ARMsocket = socket;
        qDebug() << "this first a new client";
        //socket << socket;
    }
}


void TheodoliteServer::Read() //Обработка входящих сообщений от клиента (Вехи и контроль)
{
    QObject * object = QObject::sender();// возвращает указатель на объект, который отправил сигнал
    if (!object)
    {
        return;
    }
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_13);
    //emit GetMessage("Why Are You Gay?");
    while (socket->bytesAvailable() > 0)
    {
        //emit GetMessage("Why Are You Gay?");
        if (m_nNextBlockSize == 0)
        {
            if (static_cast<unsigned long long>(socket->bytesAvailable()) < sizeof(quint32))
            {
                break;
            }
            in >> m_nNextBlockSize; //РАзмер следующего блока данных
        }

        qDebug() << "[SERVER] Считан размер следующего блока:" << m_nNextBlockSize
        << "Байт доступно в сокете:" << socket->bytesAvailable();

        if (socket->bytesAvailable() < m_nNextBlockSize)
        {
            break;
        }

        unsigned short packType;
        in >> packType;
        //srw->UpdateText();
        Vechs m_vechs;
        Control m_control;
        //QTime packetTime;

        switch (packType)
        {
            case static_cast<quint16>(MyData::VECHS) :               
                //emit EmitMessage();
                in >> m_vechs;
                qvect.push_back(m_vechs);
                break;

            case static_cast<quint16>(MyData::CONTROL):
                in >> m_control;
                qvect.push_back(m_control);
                break;
//TODO: дописать CMDFLIP
            default:
            qWarning() << "Получен поврежденный пакет неизвестного типа:"
                               << packType;
                    break;
        }
        m_nNextBlockSize = 0;
    }

    auto my_variant = *(qvect.end()-1);
    if (std::holds_alternative<Vechs>(my_variant))
    {
        emit EmitMessage(std::get<Vechs>(my_variant).to_String());
    } else if (std::holds_alternative<Control>(my_variant))
    {
        emit EmitMessage(std::get<Control>(my_variant).to_String());
    }

}


void TheodoliteServer::readAndSendPendingDatagrams()
{
    while (udp_srever_socket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(udp_srever_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udp_srever_socket->readDatagram(buffer.data(), buffer.size(),&sender, &senderPort);
        udp_req = QString(buffer);
        //emit EmitMessage(udp_req);
    }
    QByteArray data_array;
    QDataStream stream_buffer(&data_array, QIODevice::WriteOnly);
    stream_buffer << MyStruct_TheodoliteInstance;
    udp_srever_socket->writeDatagram(data_array.data(), QHostAddress::LocalHost, 8001);
}


void TheodoliteServer::stateChanged(QAbstractSocket::SocketState state) // обработчик статуса
{
    QObject * object = QObject::sender();// возвращает указатель на объект, который отправил сигнал
    if (!object)
    {
        return;
    }
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug() << state;
    if (socket == ARMsocket && state == QAbstractSocket::UnconnectedState) //Если сокет - АРМ, но он не подключился должным образом
    {
        socket->deleteLater();//Освобождаем сокет
        ARMsocket = nullptr;
    }
}


void TheodoliteServer::sockDisc()
{
    qDebug()<<"Uset disconnect";
    ARMsocket->deleteLater();
}


