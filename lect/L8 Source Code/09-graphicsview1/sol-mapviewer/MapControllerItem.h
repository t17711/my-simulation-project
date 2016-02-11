/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef MAPCONTROLLERITEM_H_
#define MAPCONTROLLERITEM_H_

#include <QtWidgets/QGraphicsWidget>

class QSlider;

class MapControllerItem : public QGraphicsWidget
{
    Q_OBJECT
public:
	explicit MapControllerItem( QGraphicsItem* p = 0, Qt::WindowFlags flags = 0 );
	~MapControllerItem();
	
Q_SIGNALS:
    void zoomChanged( int percents );
    
private Q_SLOTS:
    void zoomIn();
    void zoomOut();
    
private:
    QSlider* zoomSlider;
};

#endif /*MAPCONTROLLERITEM_H_*/
