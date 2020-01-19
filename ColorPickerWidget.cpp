﻿#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QCursor>
#include <QScreen>
#include <QMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QTimer>
#include "ColorPickerWidget.h"

ColorPickerWidget::ColorPickerWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setMouseTracking(true);

    paintCursorShapeTimer = new QTimer(this);
    paintCursorShapeTimer->setSingleShot(true);
    connect(paintCursorShapeTimer, &QTimer::timeout, [this](){this->setCursorShape();});
}

ColorPickerWidget::~ColorPickerWidget()
{
    paintCursorShapeTimer->stop();
}

void ColorPickerWidget::slotPickColor()
{
    //setCursor(Qt::CrossCursor);
    QScreen *curScreen = QGuiApplication::screenAt(mapToGlobal(QPoint(0,0)));
    resize(curScreen->size());
    move(0, 0);
}

void ColorPickerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(),QColor(255, 255, 255, 1));
}

void ColorPickerWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (true == paintCursorShapeTimer->isActive())
    {
        paintCursorShapeTimer->stop();
    }
    paintCursorShapeTimer->start(10);
}

void ColorPickerWidget::mousePressEvent(QMouseEvent *event)
{
    paintCursorShapeTimer->stop();
    if (Qt::LeftButton == event->button())
    {
        /* On Windows Vista and above grabbing a layered window,
         * which is created by setting the Qt::WA_TranslucentBackground attribute,
         * will not work. Instead grabbing the desktop widget should work. */
        QScreen *curScreen = QGuiApplication::screenAt(mapToGlobal(QPoint(0,0)));
        QPixmap pixmap = curScreen->grabWindow(QApplication::desktop()->winId(), event->x(), event->y(), 1, 1);
        QColor color = pixmap.toImage().pixelColor(0, 0);
        /* 去掉透明度的影响 */
        hide();
        emit sigColorPicked(QColor(color.red()-1, color.green()-1, color.blue()-1));
    }
    else /*if (Qt::RightButton == event->button())*/
    {
        emit sigCancelPicked();
    }
}

void ColorPickerWidget::setCursorShape()
{
    int cursorX = QCursor::pos().x();
    int cursorY = QCursor::pos().y();

    int magnifierWidth = 160;
    int magnifierHeight = 160;
    int screenshotSize = 9;

    QScreen *curScreen = QGuiApplication::screenAt(mapToGlobal(QPoint(0,0)));
    // Get image under cursor.
    int devicePixelRatio = static_cast<int>(curScreen->devicePixelRatio());
    int size = screenshotSize / devicePixelRatio;
    QPixmap pixmap = curScreen->grabWindow(QApplication::desktop()->winId(),
                                           cursorX - size / 2,
                                           cursorY - size / 2,
                                           size, size);
    pixmap = pixmap.scaled(magnifierWidth * devicePixelRatio, magnifierHeight * devicePixelRatio);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setOpacity(0.7);
    painter.setPen(QPen(QColor(255, 255, 255), 3));
    QLine xLine(QPoint(0, 0), QPoint(magnifierWidth -1, 0));
    QLine yLine(QPoint(0, 0), QPoint(0, magnifierHeight - 1));
    painter.drawLine(xLine.translated(0, magnifierHeight / 2));
    painter.drawLine(yLine.translated(magnifierWidth / 2, 0));
    painter.setOpacity(1);
    painter.setPen(QPen(QColor(255, 255, 255), 2));
    painter.drawRect(QRect(0, 0, magnifierWidth, magnifierHeight));

    setCursor(pixmap);
}
