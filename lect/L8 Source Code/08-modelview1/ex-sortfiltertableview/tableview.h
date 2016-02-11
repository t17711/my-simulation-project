/*************************************************************************
 *
 * Copyright (c) Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef TableView_H
#define TableView_H

#include <QtWidgets>

class TableView : public QWidget
{
    Q_OBJECT

public:
    explicit TableView( QWidget *parent = 0 );

    void setModel( QAbstractItemModel* model );

private slots:
    void setFilterColumn( int );

private:
    QSortFilterProxyModel *m_proxy;
    QGridLayout *m_layout;
    QTableView *m_tableView;
    QLabel *m_label;
    QLineEdit *m_lineEdit;
};


#endif
