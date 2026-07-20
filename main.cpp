#include "serverwindow.h"
#include <QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWindow w;
    //TheodoliteServer tdsrv;
    w.show();

    return a.exec();
}
