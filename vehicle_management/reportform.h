#ifndef REPORTFORM_H
#define REPORTFORM_H

#include <QWidget>
#include "mydb.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

namespace Ui {
class reportForm;
}

class reportForm : public QWidget
{
    Q_OBJECT

public:
    explicit reportForm(MyDb &db, QWidget *parent = 0);
    ~reportForm();
public slots:

    void selectAll(QString lic_plate);

private:
    Ui::reportForm *ui;

    MyDb &db;
};

#endif // REPORTFORM_H
