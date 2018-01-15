#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entry.h"
#include <QList>


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
    QUrl localURL(QString("http://10.8.250.22:30000/sql_get"));
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
    //qDebug() << "get_init: " << json;

    QJsonArray arr = json.array();

    qDebug() << "array: " << arr;

  /*  list = new <Entry> QList();
    Entry e;

    for(int i = 0; i < arr.size(); i++){
        e = new Entry();
        QJsonObject o = arr.at(i).toObject();

        e.set_fName(o.find("firstname"));
        e.set_lname(o.find("lastname"));
        e.set_ressource(o.find("resource"));

        QString sdate = o.find("reserved");
        int y =  sdate.mid(0,4).toInt();
        int m = sdate.mid(5,2).toInt();
        int d = sdate.mid(8,2).toInt();
        QDate d = new QDate(y,m,d);
        e.set_date(d);

        list.insert(i,&e);
    }
*/

}


void MainWindow::on_newRessource_clicked()
{
    //ui->label->setText(list.at(k).get_resource());
}
