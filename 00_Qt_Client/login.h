#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "entry.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    static int first;
    Entry* entry;

    void followedBy(int test);
    QString toString();

    QString getPassword() const;
    void setPassword(const QString &value);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_lineEditPW_cursorPositionChanged(int arg1, int arg2);

    bool testLogin();

private:
    Ui::Login *ui;
    int ccase;
    QString password;

};

#endif // LOGIN_H
