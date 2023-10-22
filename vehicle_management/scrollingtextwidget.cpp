#include "scrollingtextwidget.h"

ScrollingTextWidget::ScrollingTextWidget(const QString& text, QWidget *parent)
    : QWidget(parent), scrollText(text)
{
    // 设置字体和颜色
    QFont font;
    font.setPointSize(40);
    setFont(font);
    textColor = Qt::green;
    setFixedHeight(fontMetrics().height());

//    // 创建一个定时器，用于更新滚动位置
//    timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &ScrollingTextWidget::updateScrollPosition);
//    timer->start(50); // 滚动速度，单位毫秒
}
