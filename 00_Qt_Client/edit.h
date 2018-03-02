#ifndef EDIT_H
#define EDIT_H

#include <QMainWindow>
#include "entry.h"
#include "entrymodel.h"
#include "mainwindow.h"

namespace Ui {
class edit;
}

class edit : public QMainWindow
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = 0);
    ~edit();

    void setEntry(Entry* e);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_button_del_clicked();

private:
    Ui::edit *ui;
    Entry* e;
    Entry* oldEntry;
};

#endif // EDIT_H
