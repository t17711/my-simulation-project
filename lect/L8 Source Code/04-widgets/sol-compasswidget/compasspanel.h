/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef COMPASSPANEL_H
#define COMPASSPANEL_H

#include <QtWidgets>

class Compass;

class CompassPanel : public QWidget
{
    Q_OBJECT

public:
    CompassPanel(QWidget *parent = 0);

private:
    void init();

private slots:
    void updateDirection(const QString &name);

private:
    Compass *m_leftCompass;
    Compass *m_rightCompass;
    QLabel *m_direction;
};

#endif // COMPASSPANEL_H
