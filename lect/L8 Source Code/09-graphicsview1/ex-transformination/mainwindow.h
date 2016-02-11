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
#include <QtCore/QModelIndex>
#include <QtWidgets/QGraphicsView>

class CoordinateSystem;
class AnimationQueue;
class TransformationItem;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createTestObjects();

protected:
    void changeEvent(QEvent *e);
    void createItems();


private slots:
    void on_transformListWidget_doubleClicked(QModelIndex index);
    void on_actionReverse_Apply_triggered();
    void on_actionMove_Down_triggered();
    void on_actionMove_Up_triggered();
    void on_actionPlay_All_triggered();
    void on_actionZoom_out_triggered();
    void on_actionZoom_in_triggered();
    void on_actionReset_triggered();
    void on_action_Add_Transform_triggered();
    void on_actionRemove_triggered();
    void on_actionApply_triggered();
    void on_actionEdit_triggered();
    void on_action_Quit_triggered();

    void select(TransformationItem* item);
    void finishedOperation();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    CoordinateSystem *m_before;
    CoordinateSystem *m_after;
    AnimationQueue *m_animationQueue;
};

#endif // MAINWINDOW_H
