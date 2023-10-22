#ifndef MYDB_H
#define MYDB_H

#include <QDebug>

#include <QSqlDatabase>     // 数据库连接
#include <QSqlQuery>        // sql语句执行
#include <QSqlRecord>       // 数据库查询记录的管理类
#include <QSqlError>        // 数据库操作错误管理

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDateTime>        // 时间 将string 转为DATETIME

class MyDb : public QSqlDatabase
{
public:
    MyDb();
    bool initDB();
    int insertCar(const QString &lic_plate, const QString &time);
    QString getLastOne(const QString &lic_plate);
    int updateCar(const QString &lic_plate, const QString &time);
    QJsonArray  selectAll();
    QJsonArray  selectAll(const QString &lic_plate);

private:
    QSqlDatabase db; // 将数据库连接作为成员变量
};

#endif // MYDB_H
