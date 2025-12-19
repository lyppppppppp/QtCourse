#ifndef IDDATABASE_H
#define IDDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDdatabase : public QObject
{
    Q_OBJECT
public:

    static IDdatabase &getInstance()
    {
        static IDdatabase instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    QString userLogin(QString userName, QString password);

private:
    explicit IDdatabase(QObject *parent = nullptr);
    IDdatabase(IDdatabase const &) = delete;
    void operator = (IDdatabase const &)  = delete;

    QSqlDatabase database;

    void ininDatabase();


signals:

public:
    bool initPatientModel();
    int addNewPatient();
    bool searchPatient(QString filter);
    void deleteCurrentPatient();
    bool submitPatientEdit();
    void revertPatientEdit();

    QSqlTableModel *patientTabModel;//数据模型
    QItemSelectionModel *thePatientSelection;//选择模型
};

#endif // IDDATABASE_H
