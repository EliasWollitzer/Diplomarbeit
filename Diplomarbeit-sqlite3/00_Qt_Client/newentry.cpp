#include "newentry.h"
#include "ui_newentry.h"

NewEntry::NewEntry(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewEntry)
{
    //ui->dateTimeEdit->setMinimumDate(QDate::currentDate());
    ui->setupUi(this);
}

NewEntry::~NewEntry()
{
    delete ui;
}
