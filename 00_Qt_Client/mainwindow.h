#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QString>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QUrlQuery>
#include <QUrl>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtGui>
#include <QStandardItemModel>
#include <QStandardItem>



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
    void setTableConditions();

    void on_newEntryButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_addButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
