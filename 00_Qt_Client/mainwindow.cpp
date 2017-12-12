#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    QString d;
    d = QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day());
    qDebug()<<"on_calendarWidget_clicked: "<< d;

    http_get_list(d);
}

void MainWindow::http_get_list(QString date){


    QEventLoop eventLoop;
    QUrl localURL(QString("http://192.168.64.1:30000/sql_get"));
    QNetworkAccessManager mgr;

    QUrlQuery qu;
    qu.addQueryItem("date", date.toLatin1());
    qDebug() << "http_get_list: " << qu.toString();
    localURL.setQuery(qu);
    QNetworkRequest request(localURL);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(request); // GET



    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    connect(reply, SIGNAL (finished()), this , SLOT(get_init()));


    eventLoop.exec();
}

void MainWindow::get_init(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray response_data = reply->readAll();
    //reply->deleteLater();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    qDebug() << "get_init: " << json;
}
