#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT  // initialize slots

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();


private slots:
    void open();
    void close();
};

#endif // MAINWINDOW_H
