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
#include "eventlogwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EventLogWindow logger;
    logger.resize(640, 240);
    logger.move(40, 0);
    logger.show();

    Widget window;
    window.setLogAll(false); // does not log event(...) calls
    window.resize(240, 320);
    window.move(40, 320);
    window.show();

    QObject::connect(&window, SIGNAL(notifyEvent(QString, QEvent *)), &logger, SLOT(logEvent(QString, QEvent *)));

    return app.exec();
}
