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
    entry->setDatefrom(ui->dateTimeEditFROM->dateTime());
    entry->setDateto(ui->dateTimeEditTO->dateTime());
    entry->setDescription(ui->textEditDESC->toPlainText());
    //entry->setRessource(model->ressourceList.at(group->checkedId()));
    entry->setRessource("Besprechungsraum 1");


    //qDebug() << "accepted" << entry->toString() + group->id(ui->);

    model->http_post_entry(entry);
    this->close();
}

void NewEntry::on_buttonBox_rejected()
{
    this->close();
}

void NewEntry::setupRessources(){
    model = EntryModel::getInstance();
    cntRessources = model->ressourceList.size();

    if(cntRessources == 0){
        return;
    }

    group = new QButtonGroup(ui->radioLayout);
    QRadioButton* button;

    for(int i; i < cntRessources; i++){
        button = new QRadioButton(model->ressourceList.at(i));
        ui->radioLayout->addWidget(button);
        group->addButton(button);
    }


}

void NewEntry::setEntry(Entry* e){
    this->entry = e;
}
