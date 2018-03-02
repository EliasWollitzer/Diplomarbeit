#include "edit.h"
#include "ui_edit.h"



edit::edit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::edit)
{
    ui->setupUi(this);
}

edit::~edit()
{
    delete ui;
}

void edit::setEntry(Entry* e){
    this->e = e;

    oldEntry = e->copyEntry();

    ui->label_res->setText(e->getRessource());
    ui->label_depar->setText(e->getSection());
    ui->label_fn->setText(e->getFirstName());
    ui->label_ln->setText(e->getLastName());
    ui->date_from->setDateTime(e->getDatefrom());
    ui->date_to->setDateTime(e->getDateto());
    ui->lineEdit_desc->setText(e->getDescription());

}

void edit::on_buttonBox_accepted()
{
    e->setDatefrom(ui->date_from->dateTime());
    e->setDateto(ui->date_to->dateTime());
    e->setDescription(ui->lineEdit_desc->text());

    qDebug() << "edit_accepted" << e->toString();

    EntryModel* model = EntryModel::getInstance();
    model->http_update_entry(e, oldEntry);

    this->close();
}

void edit::on_buttonBox_rejected()
{
    this->close();
}

void edit::on_button_del_clicked()
{
    qDebug() << "delete button clicked" << "";

    EntryModel* model = EntryModel::getInstance();
    model->http_delete_entry(oldEntry);

    this->close();
}
