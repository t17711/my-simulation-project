/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    MainWindow mw;
    mw.show();

    return app.exec();
}
