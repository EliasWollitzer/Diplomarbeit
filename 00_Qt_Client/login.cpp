#include "login.h"
#include "ui_login.h"
#include <QtCore>
#include "newentry.h"
#include "add.h"
#include "entrymodel.h"

int Login::first = 1;
NewEntry* ne;
Add* a;

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
    //qDebug() << "buttonAccepted" << entry->toString();

    entry = new Entry();

    entry->setFirstName(ui->lineEditFN->text());
    entry->setLastName(ui->lineEditLN->text());
    entry->setSection(ui->lineEditDEPAR->text());
    this->setPassword(ui->lineEditPW->text());

    if(!this->testLogin()){
        ui->labelINFO->setText("Login data wrong");
        return;
    }

    first = 1;

    if(ccase == NEWENTRY){

        ne = new NewEntry();

        EntryModel::getInstance()->http_get_ressources();
        ne->setupRessources();
        ne->setEntry(entry);
        this->close();
        ne->show();
    }else if(ccase == ADD){
        a = new Add();
        a->setConditions();
        this->close();
        a->show();
    }
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

bool Login::testLogin(){
    //Test login data

    return true;
}

void Login::followedBy(int test){
    ccase = test;
}

QString Login::getPassword() const
{
    return password;
}

void Login::setPassword(const QString &value)
{
    password = value;
}


QString Login::toString(){
    return entry->getFirstName() + entry->getLastName() + entry->getSection() + this->getPassword();
}
