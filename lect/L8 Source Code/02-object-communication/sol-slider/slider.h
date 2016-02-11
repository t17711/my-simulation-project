/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef SLIDER_H
#define SLIDER_H

#include <QtWidgets>

class QLabel;
class QSlider;

class Slider : public QWidget
{
    Q_OBJECT
public:
    Slider(Qt::Orientation orient = Qt::Horizontal, QWidget *parent = 0);

    int value() const;
    int minimum() const;
    int maximum() const;

// Offering them as slots make the user able to connect the Qt4 way if he so prefers.
public slots:
    void setValue(int value);
    void setRange(int min, int max);
    void setMinimum(int min);
    void setMaximum(int max);

signals:
    void valueChanged(int);

protected:
    int maxWidth(int min, int max) const;

private:
    QSlider *m_slider;
    QLabel *m_display;
};

#endif // SLIDER_H
