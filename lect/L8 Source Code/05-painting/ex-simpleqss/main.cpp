/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QtWidgets>
#include "ui_form.h"

class Form : public QWidget
{
    Q_OBJECT

public:
    Form()
    {
        Ui_Form *ui = new Ui_Form;
        ui->setupUi(this);

        qApp->setStyleSheet("QLineEdit { background: red; color: white; }");
        ui->age->setStyleSheet("QLineEdit {background: blue; }");

        connect(ui->style, SIGNAL(activated(int)), this, SLOT(changeStyle(int)));
    }

protected slots:
    void changeStyle(int which)
    {
        switch (which) {
        case 0: qApp->setStyle(new QWindowsStyle);   break;
        case 1: qApp->setStyle(new QMotifStyle);     break;
        case 2: qApp->setStyle(new QPlastiqueStyle); break;
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Form widget;
    widget.show();

    qApp->setStyle(new QWindowsStyle);
    return app.exec();
}

#include "main.moc"
