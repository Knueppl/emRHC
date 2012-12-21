#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"
#include "Client.h"

#include <QSslSocket>
#include <QDomDocument>
#include <QFile>

ConnectDialog::ConnectDialog(const QString& configFile, QWidget* parent)
    : QDialog(parent),
      m_ui(new Ui::ConnectDialog),
      m_client(0),
      m_configFile(configFile)
{
    /* Setup UI */
    m_ui->setupUi(this);
    m_ui->m_spinPort->setMinimum(0);
    m_ui->m_spinPort->setMaximum(65535);
    QObject::connect(m_ui->m_buttonClose, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(m_ui->m_buttonConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));

    /* Lade Default Werte */
    QFile file(configFile);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Kann Datei " << configFile << " nicht öffnen!";
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qDebug() << "Datei " << configFile << " ist korruppt. Verwende Dafault Konfiguration.";
        file.close();
        return;
    }

    QDomNode node = doc.documentElement().firstChild();

    while (!node.isNull())
    {
        QDomElement tag = node.toElement();

        if (tag.isNull())
        {
            node = node.nextSibling();
            continue;
        }

        if (tag.tagName() == "host")
        {
            m_ui->m_lineAddress->setText(tag.text());
        }
        else if (tag.tagName() == "port")
        {
            m_ui->m_spinPort->setValue(tag.text().toInt());
        }

        node = node.nextSibling();
    }
}

ConnectDialog::~ConnectDialog(void)
{
    /* Aktuelle Werte als Default speichern */
    QFile file(m_configFile);

    if (file.open(QIODevice::WriteOnly))
    {
        QDomDocument doc;
        QDomElement root = doc.createElement("connection");
        doc.appendChild(root);

        QDomElement tag  = doc.createElement("host");
        QDomText    text = doc.createTextNode(m_ui->m_lineAddress->text());
        root.appendChild(tag);
        tag.appendChild(text);

        tag  = doc.createElement("port");
        text = doc.createTextNode(QString::number(m_ui->m_spinPort->value()));
        root.appendChild(tag);
        tag.appendChild(text);

        QTextStream stream(&file);
        doc.save(stream, 2);
        file.close();
    }
}

void ConnectDialog::connectToServer(void)
{
    delete m_client;
    m_client = 0;
    m_ui->m_labelStatus->setText("Verbindungsaufbau ...");

    /* SelfSignedCertificate akzeptieren */
    QList<QSslError> expectedSslErrors;
    QSslCertificate cert = QSslCertificate::fromPath("cacert.pem").value(0);
    expectedSslErrors.append(QSslError(QSslError::SelfSignedCertificate, cert));

    /* Neue Verbindung aufbauen */
    QSslSocket* socket = new QSslSocket;
    socket->addCaCertificate(cert);
    socket->ignoreSslErrors(expectedSslErrors);
    socket->connectToHostEncrypted(m_ui->m_lineAddress->text(), m_ui->m_spinPort->value());

    /* Warte bis Verbindung steht */
    if (!socket->waitForEncrypted(10000))
    {
        qDebug() << socket->errorString();
        m_ui->m_labelStatus->setText(QString("Fehler: ").append(socket->errorString()));
        delete socket;

        return;
    }

    m_ui->m_labelStatus->setText("Verbindung erfolgreich aufgebaut.");
    m_client = new Client(socket);
    this->disconnect(m_ui->m_buttonClose, SIGNAL(clicked()), this, SLOT(reject()));
    this->connect(m_ui->m_buttonClose, SIGNAL(clicked()), this, SLOT(accept()));
}
