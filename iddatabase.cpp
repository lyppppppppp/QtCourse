#include "iddatabase.h"
#include <QUuid>

void IDdatabase::ininDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");//添加SQL LITE数据库驱动
    QString aFile = "D:/qt作业/Lab4a.db";
    database.setDatabaseName(aFile);//设置数据库名称

    if(!database.open()) {
        qDebug() << "failed to open database";
    }else
        qDebug() << "open database is ok";
}

bool IDdatabase::initPatientModel()
{
    patientTabModel = new QSqlTableModel(this, database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit,OnRowChange
    patientTabModel->setSort(patientTabModel->fieldIndex("name"), Qt::AscendingOrder);//排序
    if(!(patientTabModel->select()))
        return false;

    thePatientSelection = new QItemSelectionModel(patientTabModel);
    return true;
}

int IDdatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(), QModelIndex());

    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount() - 1, 1);

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);
    curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTabModel->setRecord(curRecNo, curRec);

    return curIndex.row();
}

bool IDdatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

void IDdatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();//获取当前选择单元格的模型索引
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();
}

bool IDdatabase::submitPatientEdit()
{
    return patientTabModel->submitAll();
}

void IDdatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

QString IDdatabase::userLogin(QString userName, QString password)
{
    //return "loginOK";
    QSqlQuery query;//查询出当前记录的所有字段
    query.prepare("select username, password from user where username = :USER");
    query.bindValue(":USER", userName);
    query.exec();
    if(query.first() && query.value("username").isValid()){
        QString passwd = query.value("password").toString();
        if(passwd == password) {
            return "loginOK";
        }else{
            return "wrongPassword";
        }
    }else{
        qDebug() << "no such user";
        return "wrongUsername";
    }
}

IDdatabase::IDdatabase(QObject *parent) : QObject(parent)
{
    ininDatabase();
}
