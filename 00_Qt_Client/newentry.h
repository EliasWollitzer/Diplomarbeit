#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "entrymodel.h"
#include "entry.h"
#include <QRadioButton>
#include <QButtonGroup>

namespace Ui {
class NewEntry;
}

class NewEntry : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewEntry(QWidget *parent = 0);
    ~NewEntry();
    void setupRessources();
    void setName(QString fn, QString ln, QString dep);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();



private:

    Ui::NewEntry *ui;
    int cntRessources;
    EntryModel* model;
    QButtonGroup* group;

    QString firstName;
    QString lastName;
    QString department;

};

#endif // NEWENTRY_H
