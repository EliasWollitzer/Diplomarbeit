#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QLine>
#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QFormLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QSignalMapper>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QWidget *window = new QWidget; //Hauptfenster

    QFormLayout *layout = new QFormLayout(); //layout mit widgets


    name_field = new QLineEdit();
    name_field->setPlaceholderText("Name");
    name_field->setGeometry(QRect(QPoint(9,103),QSize(300,30)));

    note_field = new QLineEdit();
    note_field->setPlaceholderText("Note");
    note_field->setGeometry(QRect(QPoint(9,140),QSize(300,30)));

    submit = new QPushButton("submit");
    submit->setGeometry(QRect(QPoint(150,25),QSize(200,50)));

    search = new QPushButton("search");
    search->setGeometry(QRect(QPoint(150,600),QSize(200,30)));

    searchname = new QLineEdit();
    searchname->setPlaceholderText("Gesuchte Person");
    searchname->setGeometry(QRect(QPoint(9,190),QSize(300,30)));

    connect(submit, SIGNAL (clicked()), this, SLOT (getName()));
    connect(submit, SIGNAL (clicked()), this, SLOT (getNote()));
    connect(submit, SIGNAL (clicked()), this, SLOT (HttpPOST()));
    connect(search, SIGNAL (clicked()), this, SLOT (getSearchedname()));
    connect(search, SIGNAL (clicked()), this, SLOT (HttpGET()));

    layout->addWidget(name_field);
    layout->addWidget(note_field);
    layout->addWidget(submit);
    layout->addWidget(search);
    layout->addWidget(searchname);

    window->setLayout(layout);
    window->setFixedSize(400,300);
    window->show();
     //ui->setupUi(this);

}

static QString name;
static QString note;
static QString searchedname;

void MainWindow::HttpGET(){

    qDebug("Funktion HTTPGET");
    //HTTP

    QEventLoop eventLoop;
    QUrl localURL(QString("http://10.8.250.42:30000/sql_output"));
    QNetworkAccessManager mgr;

    QUrlQuery qu;
    qu.addQueryItem("searchedName", searchedname.toLatin1());
    qDebug()<<"Query:"<< qu.toString();
    localURL.setQuery(qu);
    QNetworkRequest request(localURL);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(request); // GET


    //connect(reply, SIGNAL (QNetworkReply::readyRead()), &eventLoop, SLOT(&MainWindow::replyFinished));
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));

    connect(reply, SIGNAL (finished()), this , SLOT(replyFinished()));

   /* connect(&mgr, SIGNAL(error(QNetworkReply::NetworkError)),
               SLOT(onError(QNetworkReply::NetworkError)));*/


    qDebug()<<"Param:"<< qu.toString();

    eventLoop.exec();
}
void MainWindow::replyFinished(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray buffer = reply->readAll();
    qDebug()<<"REPLY-ALL:"<< reply->readAll();
    QJsonDocument replyJS = QJsonDocument::fromJson(buffer);
    QJsonObject JSOreply = replyJS.object();
    qDebug()<<"REPLY:" << JSOreply["Name"];
    qDebug()<<"REPLY WORKED!!"<<replyJS.toJson();
}

QString MainWindow ::getSearchedname()
{
    searchedname = searchname->text();
    qDebug("get Name: "+searchedname.toLatin1());
    name_field->clear();
    return searchedname;
}
void MainWindow::HttpPOST(){

    qDebug("Funktion HTTPPost");
    //HTTP

    QEventLoop eventLoop;
    QUrl localURL(QString("http://10.8.250.42:30000/sql_input"));
    QNetworkRequest request(localURL);
    QNetworkAccessManager mgr;

    QUrlQuery qu;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    qu.addQueryItem("name_field", name.toLatin1());
    qu.addQueryItem("note_field", note.toLatin1());
    qDebug()<<"Query:"<< qu.toString();
    //QUrl params;
    //params.setQuery(qu);
    QNetworkReply *reply = mgr.post(request, qu.toString(QUrl::FullyEncoded).toUtf8());
    //qDebug()<<"Param:"<< params.toEncoded();
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    eventLoop.exec();
}
QString MainWindow ::getName()
{
    name = name_field->text();
    qDebug("get Name: "+name.toLatin1());
    name_field->clear();
    return name;
}

QString MainWindow ::getNote()
{
    note = note_field->text();
    qDebug("get Note: "+note.toLatin1());
    note_field->clear();
    return note;
}


MainWindow::~MainWindow()
{
    delete ui;
}
