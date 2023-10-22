#ifndef SCROLLINGTEXTWIDGET_H
#define SCROLLINGTEXTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>

class ScrollingTextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollingTextWidget(const QString& text, QWidget *parent = nullptr);

    void setScrollText(QString text){
        scrollText = text;
    }
signals:

public slots:
private slots:
    void updateScrollPosition() {
        // 更新滚动位置
        scrollPos--;
        if (scrollPos < -fontMetrics().width(scrollText)) {
            scrollPos = width(); // 当文本超出窗口时，将其移到窗口右侧重新开始滚动
        }

        // 重绘窗口
        update();
    }
protected:
    void paintEvent(QPaintEvent* ) override {
        QPainter painter(this);

        // 设置文本颜色
        QPen pen;
        pen.setColor(textColor);
        painter.setPen(pen);

        // 计算滚动文本的位置
        int textWidth = fontMetrics().width(scrollText);
        int xPos = (width() - textWidth) / 2; // 在水平方向居中显示

        // 绘制滚动文本
        painter.drawText(xPos, 0, textWidth, height(), Qt::AlignLeft | Qt::AlignVCenter, scrollText);
    }

private:
    QString scrollText;
    QColor textColor;
    int scrollPos;
    QTimer* timer;
};

#endif // SCROLLINGTEXTWIDGET_H
