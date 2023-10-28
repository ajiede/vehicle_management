## vehicle_management

## 无人停车

该项是基于QT 5.11.1(Qt Creator 4.6.2)平台开发的《无人停车场》仿真系统

**PS：**

1. 你需要先申请一个用于识别图片的百度AI接口
2. build项目的.pro文件
3. 点击Sources/widget.cpp文件
4. 在230行下  QString中补全百度AI接口

（我使用得是tokenAPI接口）

**未完成功能：**

（由于文字转语音需要付费，故未使用百度AI的短文本转语音）

1. 语音播报‘车牌信息’
2. 语音播报‘缴费金额’