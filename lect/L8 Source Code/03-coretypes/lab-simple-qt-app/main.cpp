/*************************************************************************
 *
 * Copyright (c) 2012, Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window; // our top-level window

    // ... your code here

    window.resize(480, 640);
    window.show();

    return app.exec();
}
