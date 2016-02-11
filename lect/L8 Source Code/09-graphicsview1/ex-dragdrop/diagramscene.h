/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

/* -*-C++-*- */
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QtWidgets/QGraphicsScene>

class DiagramScene : public QGraphicsScene {
public:
    explicit DiagramScene( QObject* parent=0 );
protected:
    /*reimp*/void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
    /*reimp*/void dropEvent( QGraphicsSceneDragDropEvent* event );
};

#endif /* DIAGRAMSCENE_H */

