/*************************************************************************
 *
 * Copyright (c) 2012,Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include "ui_mainwindow.h"
#include "optionsdialog.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionOpen_triggered()
{
    m_fileName = QFileDialog::getOpenFileName(this);
    if(m_fileName.isNull()) { return; }
    QFile file(m_fileName);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        ui->plainTextEdit->setPlainText(stream.readAll());
    }
}

void MainWindow::on_actionOptions_triggered()
{
    OptionsDialog dialog(this);
    dialog.exec();
}
