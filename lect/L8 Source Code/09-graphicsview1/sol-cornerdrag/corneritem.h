/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef CORNERITEM_H
#define CORNERITEM_H

#include <QtWidgets/QGraphicsRectItem>

class PictureItem;

class CornerItem : public QGraphicsRectItem
{
public:
    explicit CornerItem(Qt::Corner corner, PictureItem* parent);
    void relayout(const QRect & rect);

    // ::QGraphicsItem

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);


private:
    enum Operation {
        Off         = 0x0000,
        Rotate      = 0x0001,
        FixRotate   = 0x0002,
        Scale       = 0x0010,
        FixScale    = 0x0020,
        AllowAll    = 0xFFFF,
    };
    PictureItem* m_content;
    Qt::Corner m_corner;
    int m_size;

    int m_operation;
    double m_startRatio;

};

#endif // CORNERITEM_H
