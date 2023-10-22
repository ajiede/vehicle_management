#include "reportform.h"
#include "ui_reportform.h"

reportForm::reportForm(MyDb &db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reportForm),
    db(db)
{
    ui->setupUi(this);
}

reportForm::~reportForm()
{
    delete ui;
}

void reportForm::selectAll(QString lic_plate)
{
    QJsonArray array;
    if( lic_plate.isEmpty() ){
        array = db.selectAll();
    }else{
        array = db.selectAll(lic_plate);
    }

    // 清空tableWidget
    ui->tableWidget->clear();

    // 设置表格的列数和行数
    int rowCount = array.size();
    int columnCount = 4; // 假设有4列

    ui->tableWidget->setRowCount(rowCount);
    ui->tableWidget->setColumnCount(columnCount);

    // 设置表头
    QStringList headers;
    headers << "车牌" << "入库时间" << "出库时间" << "停车费";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 插入数据
    for (int i = 0; i < rowCount; ++i) {
        QJsonObject obj = array.at(i).toObject();
        QString col1Value = obj.value("car_name").toString();
        QString col2Value = obj.value("enter_time").toString();
        QString col3Value = obj.value("leave_time").toString();
        QString col4Value = obj.value("cost").toString();

        QTableWidgetItem *item1 = new QTableWidgetItem(col1Value);
        QTableWidgetItem *item2 = new QTableWidgetItem(col2Value);
        QTableWidgetItem *item3 = new QTableWidgetItem(col3Value);
        QTableWidgetItem *item4 = new QTableWidgetItem(col4Value);

        ui->tableWidget->setItem(i, 0, item1);
        ui->tableWidget->setItem(i, 1, item2);
        ui->tableWidget->setItem(i, 2, item3);
        ui->tableWidget->setItem(i, 3, item4);
    }

}
