#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

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

    QString getFirstName() const;
    void setFirstName(const QString &value);

    QString getLastName() const;
    void setLastName(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString toString();

    QString getDepartment() const;
    void setDepartment(const QString &value);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_lineEditPW_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::Login *ui;

    QString firstName;
    QString lastName;
    QString department;
    QString password;

};

#endif // LOGIN_H
