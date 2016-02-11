/*************************************************************************
 *
 * Copyright (c) 2012,Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

protected:
    void changeEvent(QEvent *event);

private:
    Ui::OptionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
