/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QtWidgets>
#include "slider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Slider *slider = new Slider(Qt::Horizontal);
    // Uncomment Next Line
    // slider->setRange(-100, 100);

    QLCDNumber *lcd = new QLCDNumber;
    // Ensure that the widget can grow when resizing the window, ignore this for now
    lcd->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    QObject::connect(slider, SIGNAL(valueChanged(int)), lcd, SLOT(display(int)));

    // Uncomment Next Line
    // slider->setValue(40);

    QWidget *top = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(top);
    layout->addWidget(slider);
    layout->addWidget(lcd);

    top->show();
    return app.exec();
}
