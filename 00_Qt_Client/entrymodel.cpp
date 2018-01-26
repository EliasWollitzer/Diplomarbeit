#include "entrymodel.h"


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

QVariant EntryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal){
        return QVariant();
    }

    return this->getColumnName(section);
}



QString EntryModel::getColumnName(int section) const{

    switch (section) {
    case 0:
        return "Ressource";
    case 1:
        return "Von";
    case 2:
        return "Bis";
    }
    return "";
}

void EntryModel::http_get_list(QString date){

    QEventLoop eventLoop;
    QUrl localURL(QString("http://10.8.250.22:30000/sql_get"));
    QNetworkAccessManager mgr;

    QUrlQuery qu;
    qu.addQueryItem("date", date.toLatin1());
    qDebug() << "http_get_list: " << qu.toString();

    localURL.setQuery(qu);
    QNetworkRequest request(localURL);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(request); // GET


    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    connect(reply, SIGNAL (finished()), this , SLOT(init_list()));

    eventLoop.exec();
}

void EntryModel::init_list(){

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QJsonArray arr = json.array();

    //qDebug() << "array: " << arr;

    rows = arr.size();
    columns = 3;


    qDebug() << &entrylist << endl;

    entrylist.clear();

    for(int i = 0; i < arr.size(); i++){
        Entry* e = new Entry();

        QJsonObject o = arr.at(i).toObject();
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


//BBBBBBBBBBBBBBBBBBBBBBBB





}


