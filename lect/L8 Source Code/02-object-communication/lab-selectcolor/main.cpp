/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QApplication>
#include "selectcolor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SelectColor tester;
    tester.show();

    return app.exec();
}
