/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef EDITABLEMODEL_H
#define EDITABLEMODEL_H

#include <QtWidgets>
#include "readonlymodel.h"

class EditableModel : public ReadOnlyModel
{
public:
    explicit EditableModel(QObject *parent = 0);
    ~EditableModel();
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // EDITABLEMODEL_H
