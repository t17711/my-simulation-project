/*************************************************************************
 *
 * Copyright (c) 2012,Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *event);

private slots:
    void on_actionOpen_triggered();
    void on_actionOptions_triggered();
private:
    Ui::MainWindow *ui;
    QString m_fileName;
};

#endif // MAINWINDOW_H
