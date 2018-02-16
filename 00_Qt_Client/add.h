#ifndef ADD_H
#define ADD_H

#include <QMainWindow>
#include <QButtonGroup>
#include "entrymodel.h"
#include "entry.h"



namespace Ui {
class Add;
}

class Add : public QMainWindow
{
    Q_OBJECT

public:
    explicit Add(QWidget *parent = 0);
    ~Add();

    void setConditions();

private slots:

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_ressourceButton_clicked();
    void on_personButton_clicked();
    void on_departmentButton_clicked();

private:
    Ui::Add *ui;
    QButtonGroup* group;
    int state;

    void setView();
};

#endif // ADD_H
