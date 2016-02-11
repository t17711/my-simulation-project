/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef LAZYMODEL_H
#define LAZYMODEL_H

#include <QtWidgets>

#include "readonlymodel.h"

class LazyModel : public ReadOnlyModel
{
public:
    explicit LazyModel(QObject *parent = 0);
    ~LazyModel();

    bool hasChildren(const QModelIndex &parent) const;
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
};

#endif // LAZYMODEL_H
