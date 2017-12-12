#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QUrlQuery>
#include <QUrl>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_calendarWidget_clicked(const QDate &date);
    void http_get_list(QString date);
    void get_init();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
