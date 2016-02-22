#include "mainwindow.h"

void MainWindow::setBasicActions(){
	m_ActionOpen = new QAction(QIcon("D:/COde/cqpstone/ImageProcessor/icons/open.png"), tr("&Open"), this);

	m_ActionOpen->setShortcut(tr("Ctrl+n"));
	connect(m_ActionOpen, SIGNAL(triggered()), this, SLOT(m_SetActionOpen()));

	m_ActionSave = new QAction(QIcon("D:/COde/cqpstone/ImageProcessor/icons/save.png"), tr("&Save"), this);
	m_ActionSave->setShortcut(tr("Ctrl+S"));

	connect(m_ActionSave, SIGNAL(triggered()), this, SLOT(m_SetActionSave()));

	m_ActionQuit = new QAction(tr("Exit"), this);
	connect(m_ActionQuit, SIGNAL(triggered()), this, SLOT(m_SetActionExit()));

}
// opens a dialog that grabs qstring path that has image
void MainWindow::m_SetActionOpen(){
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), "/home/newImage", tr("Image Files (*.png *.jpg *.bmp)"));
	m_imageDock->m_setImage(filename);
}
// saves file to path from dialog
void MainWindow::m_SetActionSave(){
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save Image"), "/home/new Image", tr("Image Files (*.png *.jpg *.bmp)"));
	m_imageDock->m_saveImage(filename);
}
// closes the whole thing
void MainWindow::m_SetActionExit(){
	this->close();
}