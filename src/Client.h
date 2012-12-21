#ifndef __CLIENT__
#define __CLIENT__

#include <QWidget>
#include <QAbstractSocket>

namespace Ui {
class Client;
}

class QSslSocket;
class QString;

class Client : public QWidget
{
    Q_OBJECT

public:
    enum Command {
        RealTimeOn,
        RealTimeOff
    };

    Client(QSslSocket* socket, QWidget* parent = 0);
    ~Client(void);

    void sendCommand(const Command command);

signals:
    void connectionClosed(void);

private slots:
    void disconnected(void);
    void error(QAbstractSocket::SocketError socketError);
    void receive(void);
    void test(void);

private:
    Ui::Client* m_ui;
    QSslSocket* m_socket;
    bool        m_bool;
};

#endif
