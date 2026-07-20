#pragma once


#include <QMainWindow>


namespace Ui
{
class ServerWindow;
//class TheodoliteServer;
}

class TheodoliteServer;

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();
public slots:
    void UpdateText(const QString& myText);
private:
    Ui::ServerWindow *ui;
    TheodoliteServer *thS;
};

