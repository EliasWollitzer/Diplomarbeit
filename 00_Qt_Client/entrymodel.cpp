#include "entrymodel.h"

static int b = 1;
EntryModel* EntryModel::model = nullptr;

EntryModel::EntryModel(QObject *parent) : QAbstractTableModel(parent), entrylist()
{

}

EntryModel* EntryModel::getInstance(){
    if(model == nullptr){
        model = new EntryModel();
    }
    return model;
}

int EntryModel::rowCount(const QModelIndex & /*parent*/) const
{
   return rows;
}

int EntryModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columns;
}

QVariant EntryModel::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if (role != Qt::DisplayRole)
    {
       return QVariant();
    }

    if(index.row() > 3){
        //0qDebug() << "data" << index.row()+"";
        return "";
    }

    switch(c){
        case 0: return entrylist.at(r)->getRessource();
                break;
        case 1: return entrylist.at(r)->getDate().toString();
                break;
        case 2: return entrylist.at(r)->getDate().toString();
                break;

    }

    return "";
}

void EntryModel::setTableConditions(QTableView *tv, QJsonArray arr){
    tableview = tv;
    rows = arr.size();
    columns = 3;

    qDebug() << "EntryModel" << "setTableConditions";
/*
    tableview->setColumnWidth(0,100);
    tableview->setColumnWidth(1,50);
    tableview->setColumnWidth(2,50);
*/


}

void EntryModel::formArray(QJsonArray array){
    qDebug() << &entrylist << endl;

    entrylist.clear();

    for(int i = 0; i < array.size(); i++){
        Entry* e = new Entry();

        QJsonObject o = array.at(i).toObject();
        e->setFname(o.value("firstName").toString());
        e->setLname(o.value("lastName").toString());
        e->setRessource(o.value("resource").toString());

        QString sdate = o.value("reserved").toString();
        int y =  sdate.mid(0,4).toInt();
        int m = sdate.mid(5,2).toInt();
        int d = sdate.mid(8,2).toInt();

        QDate* date = new QDate(y,m,d);
        e->setDate(*date);

        entrylist.push_back(e);
        //qDebug() << "formArray" << e->toString();
    }

    for(int i = 0; i < entrylist.size(); i++){
        qDebug() << "formArray" << entrylist[i]->toString();
    }

    //this->entrylist = list;
}



