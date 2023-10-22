#include "mydb.h"

MyDb::MyDb()
{
    // 在构造函数中初始化数据库连接
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("car.db");

}

bool MyDb::initDB()
{
    if (!db.open()) {
        QMessageBox::warning(NULL, "警告", QString("数据库创建失败: %1").arg(db.lastError().text()));
        return false;
    }

    QString sql = "CREATE TABLE IF NOT EXISTS car ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "car_name VARCHAR(255),"
                  "enter_time DATETIME,"
                  "leave_time DATETIME,"
                  "cost FLOAT);";

    QSqlQuery query;
    if (!query.exec(sql)) {
        QMessageBox::warning(NULL, "警告", QString("表格创建失败: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}

//int MyDb::insertCar(const QString &lic_plate, const QString &time)
//{
//    //qDebug() <<"1";

//    QString sql = QString("INSERT INTO car (car_name, enter_time) VALUES ('%1', '%2');").arg(lic_plate).arg(time);

//    QSqlQuery query;
//    if (!query.exec(sql)) {
//        qDebug() << "insert fail";
//        QMessageBox::warning(NULL, "警告", QString("数据添加失败: %1").arg(query.lastError().text()));
//        return -1;
//    }

//    qDebug() << "insert SUCCESS";
//    return 0;
//}

int MyDb::insertCar(const QString &lic_plate, const QString &time)
{
    QString checkSql = QString("SELECT leave_time FROM car WHERE car_name = '%1' ORDER BY id DESC LIMIT 1;").arg(lic_plate);
    QSqlQuery checkQuery;

    if (checkQuery.exec(checkSql) && checkQuery.next()) {
        // 数据已存在，可以获取 leave_time 的值
        QString leaveTime = checkQuery.value(0).toString();
        qDebug() << "last leave_time:" << leaveTime;

        if (leaveTime.isEmpty()) {
            // 如果 leave_time 为空，表示上次未出库
            qDebug() << "上次未出库";
            QMessageBox::warning(NULL, "警告", "请勿反复入库");
            qDebug() << "insert fail";
            return -1;
        } else {
            // 如果 leave_time 不为空，可以插入新数据
            QString sql = QString("INSERT INTO car (car_name, enter_time) VALUES ('%1', '%2');").arg(lic_plate).arg(time);
            QSqlQuery insertQuery;
            if (!insertQuery.exec(sql)) {
                qDebug() << "insert fail";
                QMessageBox::warning(NULL, "警告", QString("数据添加失败: %1").arg(insertQuery.lastError().text()));
                return -1;
            }
            qDebug() << "insert SUCCESS";
        }
    } else {
        // 数据不存在，可以直接插入新数据
        QString sql = QString("INSERT INTO car (car_name, enter_time) VALUES ('%1', '%2');").arg(lic_plate).arg(time);
        QSqlQuery insertQuery;
        if (!insertQuery.exec(sql)) {
            qDebug() << "insert fail";
            QMessageBox::warning(NULL, "警告", QString("数据添加失败: %1").arg(insertQuery.lastError().text()));
            return -1;
        }
        qDebug() << "insert SUCCESS";
    }

    return 0;

}



QString MyDb::getLastOne(const QString &lic_plate)
{
    QString sql = QString("SELECT enter_time, leave_time FROM car WHERE car_name='%1' "
                          "ORDER BY id DESC LIMIT 1").arg(lic_plate);
    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "查询失败";
        return QString(); // 返回空串
    }

    if (query.next()) {
        QString enterTime = query.value(0).toString();
        QString leaveTime = query.value(1).toString();

        if (leaveTime.isEmpty()) {
            qDebug() << "leave_time 为空";
            return enterTime; // 返回空串
        } else {
            qDebug() << "leave_time 不为空";
            return QString(); // 返回 enter_time
        }
    } else {
        qDebug() << "未查到";
        return QString();
    }
}


int MyDb::updateCar(const QString &lic_plate, const QString &time)
{
    QString enter_time_str = getLastOne(lic_plate);
    qDebug() << "enter_time_str" <<enter_time_str;
    qDebug() << "time" << time;
    // 以时间格式   减法  获得 时间差
    QDateTime enter_time = QDateTime::fromString(enter_time_str, "yyyy-MM-dd h:mm");
    QDateTime leave_time = QDateTime::fromString(time, "yyyy-MM-dd h:mm");

    qDebug() << "----------------------------------" ;
    qDebug() << "enter_time" << enter_time;
    qDebug() << "leave_time" << leave_time;

    if (!enter_time.isValid() || !leave_time.isValid()) {
        qDebug() << "无效的日期时间格式";
        return -1;
    }

    // 计算停车时间差
    qint64 secondsDiff = enter_time.secsTo(leave_time);
    // 类型转换，获得 停车时长
    int minCnt = static_cast<int>(secondsDiff)/60;
    int hourCnt = minCnt/60;
    if(minCnt%60>0){
        hourCnt++;
    }
    // 计算 停车费用
    int cost = 4;
    if(hourCnt>2 && hourCnt <= 10){
        cost += (hourCnt-2)*2;
    }else if(hourCnt >10){
        cost =cost + 16 + (hourCnt-10);
    }

    if(minCnt<=15){
        cost=0;
    }

    QString sql = QString("UPDATE car SET leave_time = '%1', cost = %2 "
                              "WHERE car_name = '%3' AND enter_time = '%4'")
                      .arg(time).arg(cost).arg(lic_plate).arg(enter_time_str);

    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "update fail";
        QMessageBox::warning(NULL, "警告", QString("数据更新失败: %1").arg(query.lastError().text()));
        return -1;
    }

    qDebug() << "update SUCCESS";
    return cost;
}

QJsonArray MyDb::selectAll()
{
    QJsonArray resultArray; // 用于存储查询结果

    QString sql = "SELECT * FROM car;";
    QSqlQuery query;

    if (!query.exec(sql)) {
        qDebug() << "select all fail";
        QMessageBox::warning(NULL, "警告", QString("获取全部失败: %1").arg(query.lastError().text()));
        return resultArray; // 返回空的 JSON 数组
    }

    while (query.next()) {
        QJsonObject carObject; // 为每个车辆创建一个 JSON 对象
        carObject["car_name"] = query.value("car_name").toString();
        carObject["enter_time"] = query.value("enter_time").toString();
        carObject["leave_time"] = query.value("leave_time").toString();
        carObject["cost"] = query.value("cost").toString();

        resultArray.append(carObject); // 将 JSON 对象添加到数组中
    }

    return resultArray;
}

QJsonArray MyDb::selectAll(const QString &lic_plate)
{
    QJsonArray resultArray; // 用于存储查询结果

    QString sql = QString("SELECT * FROM car WHERE car_name='%1';").arg(lic_plate);
    QSqlQuery query;

    if (!query.exec(sql)) {
        qDebug() << "select all fail";
        QMessageBox::warning(NULL, "警告", QString("获取失败: %1").arg(query.lastError().text()));
        return resultArray; // 返回空的 JSON 数组
    }

    while (query.next()) {
        QJsonObject carObject; // 为每个车辆创建一个 JSON 对象
        carObject["car_name"] = query.value("car_name").toString();
        carObject["enter_time"] = query.value("enter_time").toString();
        carObject["leave_time"] = query.value("leave_time").toString();
        carObject["cost"] = query.value("cost").toString();

        resultArray.append(carObject); // 将 JSON 对象添加到数组中
    }

    return resultArray;
}
