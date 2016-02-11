#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qstackedlayout.h>
#include <qlabel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenuBar* m_menuBar;
    QToolBar* m_toolBar;
    QLabel* m_imageDock;


    QMenu* m_file;
};

#endif // MAINWINDOW_H
