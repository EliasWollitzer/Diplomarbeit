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
    model = EntryModel::getInstance();
    model->curDate = date;

    qDebug()<<"on_calendarWidget_clicked: "<< model->dateToString(date);

    model->http_get_list();
    setTableConditions();
}



void MainWindow::on_newEntryButton_clicked()
{
    qDebug() << "newEntryButton" << "clicked";

    l = new Login();
    l->followedBy(NEWENTRY);
    l->show();
}

void MainWindow::setTableConditions(){

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
    ui->tableView->close();
    ui->tableView->show();

}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << index.row() << index.column();

}

void MainWindow::on_addButton_clicked()
{
    l = new Login();
    l->followedBy(ADD);
    l->show();
}
