#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>

#include "mydb.h"
#include "reportform.h"
#include "scrollingtextwidget.h"

//摄像头
#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPixmap>
#include <QDateTime>        // 用于给图片起名字

#include <QFileDialog>

// 使用百度ai
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHttpPart>

#include <QJsonDocument>        // 文本
#include <QJsonObject>          // json 对象
#include <QJsonArray>
#include <QJsonValue>

#include <QLabel>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_btn_openCam_clicked();

    void on_btn_sendImg_clicked();

    void on_btn_judgeImg_clicked();

    void getValue(QByteArray data);

    void updateScrollingText();

    void broadcast(QString msg);
private:
    Ui::Widget *ui;

    MyDb db;        // 数据库对象

    reportForm *form;       // 显示报表的页面

    QCamera *ca;        // 摄像头对象
    QCameraImageCapture *capture;
    QCameraViewfinder *v2;
    ScrollingTextWidget *scrollTW;

    QString fileName;
    QLabel *imageLabel;

    // 车库数量 max个  实时计数器：parkingNumber
    int max = 3;
    int parkingNumber=max;
};

#endif // WIDGET_H
