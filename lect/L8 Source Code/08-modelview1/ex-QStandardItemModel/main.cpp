/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QtGui>
#include <QtQuick>
#include <QtWidgets>

int main( int argc, char** argv ) {
    QGuiApplication app( argc, argv );

    QStandardItemModel* model = new QStandardItemModel( 10, 10 );
    for ( int row = 0; row < 10; ++row ) {
        QList<QStandardItem*> columnItems;

        for ( int col = 0; col < 10; ++col ) {
            QStandardItem *item = new QStandardItem( QString( "(%1, %2)").arg(row).arg(col) );
            columnItems << item;
        }

        model->appendRow( columnItems );
    }


    QQuickView view;

    view.engine()->rootContext()->setContextProperty( "model_", model );
    view.setSource(QUrl("qrc:///main.qml"));

    view.show();

    return app.exec();
}
