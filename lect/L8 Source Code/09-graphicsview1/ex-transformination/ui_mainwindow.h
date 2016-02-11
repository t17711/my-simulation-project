/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Sep 27 12:42:17 2012
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Add_Transform;
    QAction *action_Quit;
    QAction *actionEdit;
    QAction *actionApply;
    QAction *actionRemove;
    QAction *actionZoom_in;
    QAction *actionZoom_out;
    QAction *actionPlay_All;
    QAction *actionMove_Up;
    QAction *actionMove_Down;
    QAction *actionReverse_Apply;
    QAction *actionCenter_After_Transform;
    QAction *actionReset;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuView;
    QMenu *menuEdit;
    QMenu *menuOptions;
    QStatusBar *statusbar;
    QDockWidget *transformListDock;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QListWidget *transformListWidget;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(596, 360);
        action_Add_Transform = new QAction(MainWindow);
        action_Add_Transform->setObjectName(QStringLiteral("action_Add_Transform"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/list-add.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Add_Transform->setIcon(icon);
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName(QStringLiteral("action_Quit"));
        actionEdit = new QAction(MainWindow);
        actionEdit->setObjectName(QStringLiteral("actionEdit"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/document-edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEdit->setIcon(icon1);
        actionApply = new QAction(MainWindow);
        actionApply->setObjectName(QStringLiteral("actionApply"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/apply.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionApply->setIcon(icon2);
        actionRemove = new QAction(MainWindow);
        actionRemove->setObjectName(QStringLiteral("actionRemove"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/list-remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemove->setIcon(icon3);
        actionZoom_in = new QAction(MainWindow);
        actionZoom_in->setObjectName(QStringLiteral("actionZoom_in"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/zoom-In-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_in->setIcon(icon4);
        actionZoom_out = new QAction(MainWindow);
        actionZoom_out->setObjectName(QStringLiteral("actionZoom_out"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/zoom-out-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_out->setIcon(icon5);
        actionPlay_All = new QAction(MainWindow);
        actionPlay_All->setObjectName(QStringLiteral("actionPlay_All"));
        actionMove_Up = new QAction(MainWindow);
        actionMove_Up->setObjectName(QStringLiteral("actionMove_Up"));
        actionMove_Down = new QAction(MainWindow);
        actionMove_Down->setObjectName(QStringLiteral("actionMove_Down"));
        actionReverse_Apply = new QAction(MainWindow);
        actionReverse_Apply->setObjectName(QStringLiteral("actionReverse_Apply"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReverse_Apply->setIcon(icon6);
        actionCenter_After_Transform = new QAction(MainWindow);
        actionCenter_After_Transform->setObjectName(QStringLiteral("actionCenter_After_Transform"));
        actionCenter_After_Transform->setCheckable(true);
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QStringLiteral("actionReset"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/original.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon7);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMinimumSize(QSize(400, 0));
        graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
        graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);

        verticalLayout->addWidget(graphicsView);


        verticalLayout_3->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 596, 21));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        transformListDock = new QDockWidget(MainWindow);
        transformListDock->setObjectName(QStringLiteral("transformListDock"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        transformListWidget = new QListWidget(dockWidgetContents);
        transformListWidget->setObjectName(QStringLiteral("transformListWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(transformListWidget->sizePolicy().hasHeightForWidth());
        transformListWidget->setSizePolicy(sizePolicy1);
        transformListWidget->setMinimumSize(QSize(120, 0));
        transformListWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

        verticalLayout_2->addWidget(transformListWidget);

        transformListDock->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), transformListDock);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menu_File->addAction(actionPlay_All);
        menu_File->addAction(actionReset);
        menu_File->addAction(action_Quit);
        menuView->addAction(actionZoom_in);
        menuView->addAction(actionZoom_out);
        menuEdit->addAction(action_Add_Transform);
        menuEdit->addAction(actionRemove);
        menuEdit->addAction(actionEdit);
        menuEdit->addAction(actionApply);
        menuEdit->addAction(actionReverse_Apply);
        menuEdit->addAction(actionMove_Up);
        menuEdit->addAction(actionMove_Down);
        menuOptions->addAction(actionCenter_After_Transform);
        toolBar->addAction(actionPlay_All);
        toolBar->addAction(actionApply);
        toolBar->addAction(actionReverse_Apply);
        toolBar->addAction(action_Add_Transform);
        toolBar->addAction(actionRemove);
        toolBar->addAction(actionMove_Up);
        toolBar->addAction(actionMove_Down);
        toolBar->addAction(actionZoom_in);
        toolBar->addAction(actionZoom_out);
        toolBar->addAction(actionReset);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Transformination", 0));
        action_Add_Transform->setText(QApplication::translate("MainWindow", "&Add Transform", 0));
        action_Add_Transform->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        action_Quit->setText(QApplication::translate("MainWindow", "&Quit", 0));
        action_Quit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        actionEdit->setText(QApplication::translate("MainWindow", "Edit", 0));
#ifndef QT_NO_TOOLTIP
        actionEdit->setToolTip(QApplication::translate("MainWindow", "Edit", 0));
#endif // QT_NO_TOOLTIP
        actionEdit->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0));
        actionApply->setText(QApplication::translate("MainWindow", "Apply", 0));
#ifndef QT_NO_TOOLTIP
        actionApply->setToolTip(QApplication::translate("MainWindow", "Apply to \"after\" widget immediately", 0));
#endif // QT_NO_TOOLTIP
        actionApply->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0));
        actionRemove->setText(QApplication::translate("MainWindow", "Remove", 0));
#ifndef QT_NO_TOOLTIP
        actionRemove->setToolTip(QApplication::translate("MainWindow", "Remove", 0));
#endif // QT_NO_TOOLTIP
        actionRemove->setShortcut(QApplication::translate("MainWindow", "Del", 0));
        actionZoom_in->setText(QApplication::translate("MainWindow", "Zoom in", 0));
        actionZoom_in->setShortcut(QApplication::translate("MainWindow", "Ctrl++", 0));
        actionZoom_out->setText(QApplication::translate("MainWindow", "Zoom out", 0));
        actionZoom_out->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0));
        actionPlay_All->setText(QApplication::translate("MainWindow", "Play All", 0));
        actionMove_Up->setText(QApplication::translate("MainWindow", "Move Up", 0));
        actionMove_Down->setText(QApplication::translate("MainWindow", "Move Down", 0));
        actionReverse_Apply->setText(QApplication::translate("MainWindow", "Reverse Apply", 0));
        actionReverse_Apply->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0));
        actionCenter_After_Transform->setText(QApplication::translate("MainWindow", "Center After Transform", 0));
        actionReset->setText(QApplication::translate("MainWindow", "Reset to Original", 0));
        actionReset->setShortcut(QApplication::translate("MainWindow", "F5", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Items", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0));
        transformListDock->setWindowTitle(QApplication::translate("MainWindow", "TransformList", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
