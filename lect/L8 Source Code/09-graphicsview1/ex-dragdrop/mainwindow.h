/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

/* -*-C++-*- */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class ToolBox;
class QGraphicsView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow( QWidget* parent = 0 );
private:
    ToolBox* m_toolbox;
    QGraphicsView* m_view;
};

#endif /* MAINWINDOW_H */

