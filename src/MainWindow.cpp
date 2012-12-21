#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ConnectDialog.h"
#include "Client.h"

#include <QSslSocket>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_client(0)
{
    m_ui->setupUi(this);
    this->setWindowTitle("Betrachter Trocknung");

    QMenuBar* menuBar = this->menuBar();
    QMenu* menu = menuBar->addMenu("Trocknung");
    QAction* action = menu->addAction("Verbinden");
    QObject::connect(action, SIGNAL(triggered()), this, SLOT(connecT()));

    action = menu->addAction("Trennen");
    QObject::connect(action, SIGNAL(triggered()), this, SLOT(disconnecT()));
}

MainWindow::~MainWindow(void)
{
    delete m_client;
}

void MainWindow::connecT(void)
{
    ConnectDialog dialog("connection.conf", this);

    if (dialog.exec())
    {
        delete m_client;
        m_client = dialog.socket();
        m_client->show();
    }
}

void MainWindow::disconnecT(void)
{
    delete m_client;
    m_client = 0;
}
