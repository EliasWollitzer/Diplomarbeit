#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QMainWindow>

namespace Ui {
class NewEntry;
}

class NewEntry : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewEntry(QWidget *parent = 0);
    ~NewEntry();

private:
    Ui::NewEntry *ui;
};

#endif // NEWENTRY_H
