#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	QString temp = "D:/wallpapers/1.jpg";  // just initial stuff to see if image opens

	m_imageDock = new image_area(temp);    

    m_menuBar = new QMenuBar(0);
    m_toolBar = new QToolBar(0);

	setCentralWidget(m_imageDock);
    setBasicActions(); // do thois before set file and toolbar
    setFileMenu();
    setToolbarIcons();
    setToolbarIcons();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setToolbarIcons(){
    m_toolBar->addAction(m_ActionOpen);
    m_toolBar->addAction(m_ActionSave);
    m_toolBar->setIconSize(QSize(50,50));
	addToolBarBreak();
	addToolBar(m_toolBar);
	addToolBarBreak();
}
