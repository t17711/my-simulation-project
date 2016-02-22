#include "mainwindow.h"

void MainWindow::setFileMenu(){
	m_MenuFile = menuBar()->addMenu(tr("&File"));
	m_MenuFile->addAction(m_ActionOpen);
	m_MenuFile->addAction(m_ActionSave);
	m_MenuFile->addAction(m_ActionQuit);

}

