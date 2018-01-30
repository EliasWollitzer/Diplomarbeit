#include "newentry.h"
#include "ui_newentry.h"
#include <QtCore>





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


void NewEntry::on_buttonBox_accepted()
{
    Entry* e;
    e->setFirstName(firstName);
    e->setLastName(lastName);
    e->setDatefrom(ui->dateTimeEditFROM->dateTime());
    e->setDateto(ui->dateTimeEditTO->dateTime());
    e->setDescription(ui->textEditDESC->textCursor().selectedText());
    e->setSection(department);
    //e->setRessource(model->ressourceList.at(group->checkedId()));
    e->setRessource("");

    qDebug() << "accepted" << e->toString();

    model->http_post_entry(e);
    this->close();
}

void NewEntry::on_buttonBox_rejected()
{
    this->close();
}

void NewEntry::setupRessources(){
    model = EntryModel::getInstance();
    cntRessources = 3; //model->ressourceList.size();

    if(cntRessources == 0){
        return;
    }

    group = new QButtonGroup(ui->radioLayout);
    QRadioButton* button;

    for(int i; i < cntRessources; i++){
        button = new QRadioButton("test");
        ui->radioLayout->addWidget(button);
        group->addButton(button);
    }


}

void NewEntry::setName(QString fn, QString ln, QString depar){
    firstName = fn;
    lastName = ln;
    department = depar;
}
