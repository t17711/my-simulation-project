/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget window;
    window.show();
    return app.exec();
}
