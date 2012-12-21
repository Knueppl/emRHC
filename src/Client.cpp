#include "Client.h"
#include "ui_Client.h"

#include <QErrorMessage>
#include <QSslSocket>
#include <QString>

Client::Client(QSslSocket* socket, QWidget* parent)
    : QWidget(parent),
      m_ui(new Ui::Client),
      m_socket(socket)
{
    m_ui->setupUi(this);
    this->connect(m_ui->m_buttonTest, SIGNAL(clicked()), this, SLOT(test()));

    this->connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    this->connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    this->connect(m_socket, SIGNAL(readyRead()), this, SLOT(receive()));
}

Client::~Client(void)
{
    this->disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    this->disconnect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
//    m_socket->disconnectFromHost();
//    m_socket->waitForDisconnected(10000);
    delete m_socket;
}

void Client::disconnected(void)
{
    qDebug() << __PRETTY_FUNCTION__;
    emit this->connectionClosed();
}

void Client::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << __PRETTY_FUNCTION__;
    QErrorMessage msg;
    msg.showMessage(m_socket->errorString());
    m_socket->disconnectFromHost();
}

void Client::sendCommand(const Command command)
{
    switch (command)
    {
    case RealTimeOn:
        m_socket->write("<command>RealTimeOn</command>\n");
        break;

    case RealTimeOff:
        m_socket->write("<command>RealTimeOff</command>\n");
        break;

    default:
        break;
    }
}

void Client::receive(void)
{
    static QString buffer;

    buffer.append(m_socket->readLine());
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << buffer;

    int tOpenB = buffer.indexOf('<');
    int tOpenE = buffer.indexOf('>');
    int tCloseB = buffer.lastIndexOf('<');
    int tCloseE = buffer.lastIndexOf('>');

    QStringRef tag = buffer.midRef(tOpenB + 1, tOpenE - tOpenB - 1);
    QStringRef tagClose = buffer.midRef(tCloseB + 1, tCloseE - tCloseB - 1);
    QString data = buffer.mid(tOpenE + 1, tCloseB - tOpenE - 1);

    if (tag == "datarealtime")
    {
        if (tagClose == "/datarealtime")
        {
            QStringList list = data.split(",", QString::SkipEmptyParts);

            buffer.clear();
        }
    }
}

void Client::test(void)
{
    this->sendCommand(RealTimeOn);
}
