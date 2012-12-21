#include "MainWindow.h"

#include <QApplication>
#include <QSslSocket>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    if (!QSslSocket::supportsSsl()) {
        qDebug() <<  "Secure Socket Client: This system does not support OpenSSL.";
        return -1;
    }

    MainWindow m;
    m.show();

    return app.exec();
}
