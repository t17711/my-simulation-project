#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <image_area.h>

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <qstackedlayout.h>
#include <qlabel.h>
#include <QFileDialog>
#include <QIcon>
#include <QSizePolicy>

#include "ui_mainwindow.h"
#include "image_area.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


    void				setBasicActions();
    void				setToolbarIcons();

    void				setFileMenu();
    void				setEditMenu();
    void				setHelpMenu();
    void				openAction();


public slots:
	void				m_SetActionOpen();
	void				m_SetActionSave();
	void				m_SetActionExit();

private:

    Ui::MainWindow*		ui;
    
	QMenuBar*			m_menuBar;

    QToolBar*			m_toolBar;

	image_area *		m_imageDock;

	QMenu*				m_MenuFile;
	QMenu*				m_MenuEdit;
	QMenu*				m_MenuHelp;

    QAction*			m_ActionOpen;
	QAction*			m_ActionSave;
	QAction*			m_ActionQuit;
};


#endif // MAINWINDOW_H
