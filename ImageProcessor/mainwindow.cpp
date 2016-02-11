#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_imageDock = new QLabel(tr("Main image"));
    m_menuBar = new QMenuBar(0);
    m_toolBar = new QToolBar(0);

    setCentralWidget(m_imageDock);
    m_file =  menuBar()->addMenu("&file");

}

MainWindow::~MainWindow()
{
    delete ui;
}
