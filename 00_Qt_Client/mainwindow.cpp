#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entry.h"
#include "entrymodel.h"
#include <QList>
#include <Qt>
#include "newentry.h"
#include <QApplication>
#include "login.h"



EntryModel* model;
Login* l;

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

    model = EntryModel::getInstance();
    model->http_get_list(d);
    setTableConditions();
}



void MainWindow::on_newEntryButton_clicked()
{
    QDateTime *dt = new QDateTime();
    dt->setDate(QDate::currentDate());
    dt->setTime(QTime::currentTime());
    qDebug() << "datetime" << dt->toString();

    l = new Login();
    l->show();
}

void MainWindow::setTableConditions(){

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
    ui->tableView->show();
}

