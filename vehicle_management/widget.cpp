#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 给窗口设置标签  和 名称
    this->setWindowIcon(QIcon(":/img/停车.png"));
    this->setWindowTitle("莓用停车场");
    // 按钮前面的icon
    ui->btn_judgeImg->setIcon(QIcon(":/img/judge.png"));
    ui->btn_sendImg->setIcon(QIcon(":/img/file.png"));
    ui->btn_openCam->setIcon(QIcon(":/img/cam.png"));
    ui->pushButton->setIcon(QIcon(":/img/ruku.png"));
    ui->pushButton_2->setIcon(QIcon(":/img/chuku.png"));
    ui->pushButton_5->setIcon(QIcon(":/img/biaoge.png"));

    // 设置背景图
    QString imagePath = "D:/hqyj/msg/day/QT/my/project/vehicle_management/img/bg5.webp";
    QPixmap originalBackground(imagePath);
    // 获取主页面的大小
    QSize widgetSize = this->size();
    // 将背景图像缩放以适应主页面的大小
    QPixmap scaledBackground = originalBackground.scaled(widgetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // 创建一个QPalette对象，设置背景图像
    QPalette palette;
    palette.setBrush(QPalette::Background, scaledBackground);
    // 设置主页面的背景
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 初始化数据库
    db.initDB();

    // 将报表要显示的ui指定为ui->widget_2 上；并隐藏
    form = new reportForm(db,ui->widget_2);
    form->hide();


    // 获取摄像头
    QList<QCameraInfo> infos = QCameraInfo::availableCameras();
    // 获取可用摄像头信息
    QString camera = infos.value(0).deviceName();

    // 使用第一个摄像头，指定父对象
    ca =new QCamera(camera.toUtf8() ,ui->widget_2);
    ui->widget_2->show();

    // 视频流
    v2 = new QCameraViewfinder(ui->widget_2);
    v2->setGeometry(ui->widget_2->geometry());
    ca->setViewfinder(v2);
    v2->hide();

    // 设置显示停车等信息
    scrollTW = new ScrollingTextWidget(QString(),ui->widget_2);
    updateScrollingText();

    // 创建一个label 用于显示 车牌图片
    imageLabel = new QLabel(ui->widget_2);
    imageLabel->setGeometry(ui->widget_2->geometry());

}

Widget::~Widget()
{
    delete ui;
}

// 入库
void Widget::on_pushButton_clicked()
{
    // 如果有报表页面显示  则隐藏
    form->hide();
    // 将当前设置的时间 作为入库的时间
    QString time = ui->edt_time->text();
    QString lic_plate = ui->lineEdit_car->text();
    qDebug() << "入库";
    qDebug() << time;
    qDebug() << lic_plate;

    // 将车牌 信息 播报出来
    //broadcast(lic_plate);

    // 入库信息判断
    if(parkingNumber <= 0){
        QMessageBox::warning(this,"警告","当前车位已满");
    }else if( -1 == db.insertCar(lic_plate,time)){
        QMessageBox::warning(this,"警告","入库失败");
    }else{
        // 车位数量减1
        parkingNumber--;
        QMessageBox::information(this,"提示","入库成功");
    }

    // 更新  停车数量信息
    updateScrollingText();
}

// 出库
void Widget::on_pushButton_2_clicked()
{

    form->hide();

    QString time = ui->edt_time->text();
    QString lic_plate = ui->lineEdit_car->text();
    qDebug() << "出库";
    qDebug() << time;
    qDebug() << lic_plate;
    // 获得停车费，便于后续使用，将文字转为语音
    int cost = db.updateCar(lic_plate,time);

    if( -1 == cost ){
        qDebug() << "出库失败";
        QMessageBox::warning(this,"警告","出库无效");
        return ;
    }else{
        // 当出库后的数量  大于标准的 结果时，必然出现 一辆车出库多次情况
        if(++parkingNumber <=max ){
            if(0 == cost){
                QMessageBox::information(this,"提示","当前停车时长小于15min，不收取费用");
            }else{
                QMessageBox::information(this,"提示","出库成功，请及时缴费");
                qDebug() << "cost:" << cost;
                QMessageBox::information(this,"缴费",QString("请缴费:%1元").arg(cost));
            }
        }else{
            parkingNumber = max;
            QMessageBox::warning(this,"警告","出库错误，请勿反复出库");
        }
    }

    // 将金额播放
    //broadcast(QString(cost));

    // 更新信息
    updateScrollingText();
}

// 报表
void Widget::on_pushButton_5_clicked()
{
    // 将当前可能会出现的界面影藏
    imageLabel->hide();
    scrollTW->hide();
    // 全查 并显示
    form->selectAll(ui->lineEdit_car->text());
    form->show();
}

// 打开摄像头
void Widget::on_btn_openCam_clicked()
{
    imageLabel->hide();
    form->hide();
    scrollTW->hide();

    // 根据按钮上的文本信息 做出不同的操作（互斥if，只能出现一种情况，完成一种操作）
    if(ui->btn_openCam->text() == "打开摄像头"){
        v2->show();     // 显示摄像头界面
        ca->start();        // 运行摄像头
        ui->btn_openCam->setText("拍照");     // 修改按钮的文本
        qDebug() << "打开摄像头";
        return;         // 提前结束
    }else if( ui ->btn_openCam->text() == "拍照" ){
        qDebug() << "拍照";
        //截取用户打卡时的图片
        capture = new QCameraImageCapture(ca);
        QDateTime time = QDateTime::currentDateTime();      // 获取当前时间
        QString nameseed = time.toString("yy-MM-dd hh-mm-ss");      // 将当前时间格式化，
        QString savePath = QCoreApplication::applicationDirPath() + "/" + nameseed + ".jpg";
        qDebug() << savePath;
        fileName = savePath;        // 将图片的路径保存下来，便于后面，上传识别
        capture->capture(savePath);         // 保存图片
        //capture->capture("D://hqyj//msg//day//QT//my//project//"+nameseed+".jpg");
        QMessageBox::information(this, "完成", "拍照完成");
    }
}

// 选择文件
void Widget::on_btn_sendImg_clicked()
{
    updateScrollingText();
    imageLabel->hide();
    form->hide();
    // 选择照片上传，只显示jpg和png格式（如果想添加新格式 只需要按照格式[;;.webg(*.webg)]的格式添加）
    fileName = QFileDialog::getOpenFileName(this,"选择文件","./debug",".jpg(*.jpg);;.png(*.png)");
    if( fileName.isEmpty() ){
        QMessageBox::information(this,"提示","你还未选择图片");
        return ;
    }
    // 选择完照片  还需要点击上传  才能进行识别
}

// 上传文件
void Widget::on_btn_judgeImg_clicked()
{
    updateScrollingText();
    imageLabel->hide();
    form->hide();
    v2->hide();

    // 创建一个多部分HTTP请求
    // 请求信息体，将信息包装在其中
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QFile imageFile(fileName);      // 根据前面拍照或选择本地文件信息（fileName） 获取图片
    if (!imageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "图片打开失败";
        delete multiPart;
        return;
    }
    QByteArray imageData = imageFile.readAll();
    // 读取图像文件并将其转换为Base64编码
    // 根据百度AI的要求 图片是base64 或者 直接是 url（由于是本地路径所以不存在url路径）
    QString base64Image = QString::fromLatin1(imageData.toBase64());

    // 添加Base64图像数据到请求
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\""));
    imagePart.setBody(base64Image.toLatin1());
    multiPart->append(imagePart);


    // 使用百度token（如何获取，参考百度api接口测试 给出的视频讲解）
    QUrl url =  QString("填入百度AI的api接口");
    // 创建HTTP请求对象
    QNetworkRequest request(url);

    // 发送POST请求，并将多部分HTTP请求作为参数传递
    QNetworkReply *reply = manager->post(request, multiPart);

    // 处理请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 请求成功，处理响应数据
            QByteArray responseData = reply->readAll();
            // 在这里处理responseData中的信息
            //qDebug() << "Response:" << responseData;
            getValue(responseData);
        } else {
            // 请求失败，处理错误
            qDebug() << "Error:" << reply->errorString();
        }

        // 释放资源
        reply->deleteLater();
        delete multiPart;
    });
}

// 处理回应的结果，使用QJson等库
void Widget::getValue(QByteArray data)
{
    // 将QByteArray转为QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qDebug() << "Invalid JSON data";
        return;
    }

    // 获取根对象
    QJsonObject rootObj = doc.object();

    // 获取"words_result"对象
    QJsonObject wordsResultObj = rootObj.value("words_result").toObject();
    // 获取"number"的值
    QString number = wordsResultObj.value("number").toString();
    // 获取"color"的值
    QString color = wordsResultObj.value("color").toString();

    // 获取车牌的 number 和 颜色  并显示在lineEdit上
    qDebug() << "number:" << number;
    qDebug() << "color:" << color;
    ui->lineEdit_car->clear();
    ui->lineEdit_car->setText(number);
    QString styleSheet = QString("background-color: %1; color: white;font:14pt;").arg(color);
    ui->lineEdit_car->setStyleSheet(styleSheet);

    // 将fileName显示在label上
    QPixmap image(fileName);
    imageLabel->setPixmap(image);
    imageLabel->setScaledContents(true);
}

// 更新车库信息
void Widget::updateScrollingText() {
    QString text;
    if (parkingNumber != 0) {
        text = QString("当前剩余车位%1个").arg(parkingNumber);
    } else {
        text = QString("当前车位已满");
    }

    scrollTW->setScrollText(text);
    scrollTW->setGeometry(ui->widget_2->geometry());
}

// 播报信息
void Widget::broadcast(QString msg)
{
    qDebug() <<msg;
//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

//    QUrl url =  QString("短文本语音合成API——token");
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//    QJsonObject jsonData;
//    jsonData["info"] = QString(msg);

//    QJsonDocument jsonDoc(jsonData);
//    QByteArray jsonDataByteArray = jsonDoc.toJson(QJsonDocument::Compact);

//    // 发送 POST 请求，并将 JSON 数据作为请求体
//    QNetworkReply *reply = manager->post(request, jsonDataByteArray);

//    // 返回结果 参考百度AI的 ‘短文本合成’ 的使用，
//    connect(reply, &QNetworkReply::finished, this, [=]() {
//        if (reply->error() == QNetworkReply::NoError) {
//            // 请求成功，处理响应数据
//            QByteArray responseData = reply->readAll();
//            // 在这里处理 responseData 中的信息
//            // qDebug() << "Response:" << responseData;
//        } else {
//            // 请求失败，处理错误
//            qDebug() << "Error:" << reply->errorString();
//        }

//        // 释放资源
//        reply->deleteLater();
//    });

}
