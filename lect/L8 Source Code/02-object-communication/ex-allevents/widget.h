/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void setLogAll(bool reduced);

protected:
    // central event handler
    bool event(QEvent *event);
    // paint event
    void paintEvent(QPaintEvent *event);
    // input events
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    // focus events
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    // geometry events
    void resizeEvent(QResizeEvent *event);
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
    // other events
    void closeEvent(QCloseEvent *event);

signals:
    void notifyEvent(const QString &message, QEvent *event);

private:
    bool m_logAll;
};

#endif
