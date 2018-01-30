#include "login.h"
#include "ui_login.h"
#include <QtCore>
#include "newentry.h"
#include "entrymodel.h"

int Login::first = 1;
NewEntry* ne;

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_buttonBox_accepted()
{
    this->setFirstName(ui->lineEditFN->text());
    this->setLastName(ui->lineEditLN->text());
    this->setPassword(ui->lineEditPW->text());

    qDebug() << "accepted" << this->toString();
    first = 1;
    this->close();
    ne = new NewEntry();

    //EntryModel::getInstance()->http_get_ressources();
    ne->setupRessources();
    ne->setName(firstName, lastName, department);
    ne->show();

}

void Login::on_buttonBox_rejected()
{
    first = 1;
    this->close();
}

void Login::on_lineEditPW_cursorPositionChanged(int arg1, int arg2)
{
    if(first){
        qDebug() << "login" << "cursorChanged";
        ui->lineEditPW->setEchoMode(QLineEdit::Password);
        ui->lineEditPW->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
        first = 0;
    }


}

QString Login::getDepartment() const
{
    return department;
}

void Login::setDepartment(const QString &value)
{
    department = value;
}

QString Login::getPassword() const
{
    return password;
}

void Login::setPassword(const QString &value)
{
    password = value;
}

QString Login::getLastName() const
{
    return lastName;
}

void Login::setLastName(const QString &value)
{
    lastName = value;
}

QString Login::getFirstName() const
{
    return firstName;
}

void Login::setFirstName(const QString &value)
{
    firstName = value;
}

QString Login::toString(){
    return firstName + " " + lastName + " " + password;
}
