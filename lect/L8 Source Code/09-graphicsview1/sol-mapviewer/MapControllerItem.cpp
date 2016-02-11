/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include "MapControllerItem.h"

#include <QtWidgets/QGraphicsLinearLayout>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QPushButton>

MapControllerItem::MapControllerItem( QGraphicsItem* p, Qt::WindowFlags flags )
  : QGraphicsWidget( p, flags ), zoomSlider(0)
{
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout( Qt::Vertical );
    setLayout( layout );

    QPushButton* zoomInButton = new QPushButton( QLatin1String("+") );
    zoomInButton->setFixedWidth(50);
    zoomInButton->setAttribute( Qt::WA_NoSystemBackground );
    QGraphicsProxyWidget* zoomInButtonProxy = new QGraphicsProxyWidget;
    zoomInButtonProxy->setWidget( zoomInButton );
    layout->addItem( zoomInButtonProxy );

    zoomSlider = new QSlider( Qt::Vertical );
    zoomSlider->setAttribute( Qt::WA_NoSystemBackground );
    zoomSlider->setRange( 10, 500 );
    zoomSlider->setValue( 100 );
    zoomSlider->setSingleStep( 20 );
    zoomSlider->setPageStep( 100 );
    zoomSlider->setTickPosition( QSlider::TicksBothSides );
    QGraphicsProxyWidget* zoomSliderProxy = new QGraphicsProxyWidget(this);
    zoomSliderProxy->setWidget( zoomSlider );    
    layout->addItem( zoomSliderProxy );    

    QPushButton* zoomOutButton = new QPushButton( QLatin1String("-") );
    zoomOutButton->setFixedWidth(50);
    zoomOutButton->setAttribute( Qt::WA_NoSystemBackground );
    QGraphicsProxyWidget* zoomOutButtonProxy = new QGraphicsProxyWidget;
    zoomOutButtonProxy->setWidget( zoomOutButton );
    layout->addItem( zoomOutButtonProxy );

    connect( zoomSlider, SIGNAL(valueChanged(int)), this, SIGNAL(zoomChanged(int)) );
    connect( zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()) );
    connect( zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()) );
}

MapControllerItem::~MapControllerItem()
{
}

void MapControllerItem::zoomIn()
{
    zoomSlider->setValue( zoomSlider->value() + zoomSlider->singleStep() );
}

void MapControllerItem::zoomOut()
{
    zoomSlider->setValue( zoomSlider->value() - zoomSlider->singleStep() );    
}
