#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "server.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);

    thS = new TheodoliteServer(this);
    connect(thS, SIGNAL(EmitMessage(QString)), this, SLOT(UpdateText(QString)));
    //connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
}

void ServerWindow::UpdateText(const QString& myText)
{
    ui->textEdit->append(myText);

}

ServerWindow::~ServerWindow()
{
    delete ui;
    delete thS;
}
