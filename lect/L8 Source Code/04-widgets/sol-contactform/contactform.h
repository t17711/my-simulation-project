/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef CONTACTFORM_H
#define CONTACTFORM_H

#include <QtWidgets>

class ContactForm : public QWidget
{
    Q_OBJECT

public:
    ContactForm(QWidget *parent = 0);
    void createContactGroup();
    void createDetailsGroup();

private:
    QGroupBox *m_contactGroup;
    QGroupBox *m_detailsGroup;
};

#endif // CONTACTFORM_H
