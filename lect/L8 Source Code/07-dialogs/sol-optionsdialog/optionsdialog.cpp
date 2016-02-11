/*************************************************************************
 *
 * Copyright (c) 2012,Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include "ui_optionsdialog.h"
#include "optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
