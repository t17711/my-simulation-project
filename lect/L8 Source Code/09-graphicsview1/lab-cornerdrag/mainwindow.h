/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

namespace Ui {
    class MainWindow;
}

class QGraphicsScene;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addPictures(QStringList);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;

private slots:
    void on_actionQuit_triggered();
    void on_action_Open_Image_triggered();
};

#endif // MAINWINDOW_H
