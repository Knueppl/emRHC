#ifndef __CONNECT_DIALOG__
#define __CONNECT_DIALOG__

#include <QDialog>
#include <QString>

namespace Ui {
class ConnectDialog;
}

class Client;

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectDialog(const QString& configFile = QString("connection.conf"), QWidget* parent = 0);
    ~ConnectDialog(void);

    /* Wird connect das Zweite mal aufgerufen, wird Client* gelöscht. */
    Client* socket(void) const { return m_client; }

private slots:
    void connectToServer(void);

private:
    Ui::ConnectDialog* m_ui;
    Client*            m_client;
    QString            m_configFile;
};

#endif
