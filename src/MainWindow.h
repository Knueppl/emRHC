#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QMainWindow>
#include <QVector>

namespace Ui {
class MainWindow;
}

class Client;
class QStringList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow(void);

private slots:
    void connecT(void);
    void disconnecT(void);

private:
    Ui::MainWindow* m_ui;
    Client*         m_client;
};

#endif
