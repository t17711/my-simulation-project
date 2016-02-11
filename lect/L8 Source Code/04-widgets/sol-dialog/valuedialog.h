/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef VALUEDIALOG_H
#define VALUEDIALOG_H

#include <QtWidgets>

/*
 A dialog, which displays a spinbox and a slider.
 Both are value changes are connected to each others set value.
 Emits valueChanged if sliders value is changed.
 Provides also a setValue, which set's value on the slider.
 Additional is shows two dialog buttons Ok and Cancel.

 The dialog only closes, if the value is < 50, otherwise a message box is shown.
 */
class ValueDialog : public QDialog
{
    Q_OBJECT

public:
    ValueDialog(QWidget *parent = 0);

public slots:
    void setValue(int value);

protected:
    void accept();

signals:
    void valueChanged(int value);

private:
    QSpinBox *spin;
    QSlider *slider;
    QDialogButtonBox *box;
};

#endif // VALUEDIALOG_H
