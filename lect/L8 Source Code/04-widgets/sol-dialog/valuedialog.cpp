/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include "valuedialog.h"

ValueDialog::ValueDialog(QWidget *parent)
    : QDialog(parent)
{
    spin = new QSpinBox;
    slider = new QSlider(Qt::Horizontal);
    connect(spin, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), spin, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));

    box = new QDialogButtonBox;
    box->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(spin);
    layout->addWidget(slider);
    layout->addWidget(box);
    setLayout(layout);
}

void ValueDialog::setValue(int value)
{
    spin->setValue(value);
}

void ValueDialog::accept()
{
    if(slider->value() < 50)
        QDialog::accept();
    else
        QMessageBox::information(this, "Value Check", "Value must be below 50");
}
