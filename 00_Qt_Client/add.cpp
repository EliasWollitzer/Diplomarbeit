#include "add.h"
#include "ui_add.h"

Add::Add(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Add)
{
    ui->setupUi(this);
}

Add::~Add()
{
    delete ui;
}

void Add::setConditions(){

    group = new QButtonGroup(ui->radioLayout);

    group->addButton(ui->ressourceButton);
    group->addButton(ui->personButton);
    group->addButton(ui->departmentButton);

    ui->ressourceButton->setChecked(true);
    state = RESSOURCE;
    setView();
}

void Add::setView(){

    switch(state){
        case RESSOURCE:     ui->label_1->setText("Ressource name");
                            ui->label_2->setVisible(false);
                            ui->lineEdit_2->setVisible(false);
                            ui->label_3->setVisible(false);
                            ui->lineEdit_3->setVisible(false);
                            break;

        case PERSON:        ui->label_1->setText("First name");
                            ui->label_2->setVisible(true);
                            ui->label_2->setText("Last name");
                            ui->lineEdit_2->setVisible(true);
                            ui->label_3->setVisible(true);
                            ui->label_3->setText("Department");
                            ui->lineEdit_3->setVisible(true);
                            break;

        case DEPARTMENT:    ui->label_1->setText("Department name");
                            ui->label_2->setVisible(false);
                            ui->lineEdit_2->setVisible(false);
                            ui->label_3->setVisible(false);
                            ui->lineEdit_3->setVisible(false);
                            break;
    }
}

void Add::on_buttonBox_accepted()
{
    QList <QString> list;

    switch(state){
        case RESSOURCE:     list.append(ui->lineEdit_1->text());
                            break;

        case DEPARTMENT:    list.append(ui->lineEdit_1->text());
                            list.append("");
                            break;

        case PERSON:        list.append(ui->lineEdit_1->text());
                            list.append(ui->lineEdit_2->text());
                            list.append(ui->lineEdit_3->text());
                            break;
    }

    EntryModel::getInstance()->http_add_component(list);
    this->close();
}

void Add::on_buttonBox_rejected()
{
    this->close();
}

void Add::on_ressourceButton_clicked()
{
    state = RESSOURCE;
    setView();
}

void Add::on_personButton_clicked()
{
    state = PERSON;
    setView();
}

void Add::on_departmentButton_clicked()
{
    state = DEPARTMENT;
    setView();
}
