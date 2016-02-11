/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#ifndef READONLYMODEL_H
#define READONLYMODEL_H

#include <QtWidgets>

class Node;

class ReadOnlyModel : public QAbstractItemModel
{
public:
    explicit ReadOnlyModel(QObject *parent = 0);
    ~ReadOnlyModel();
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setRootNode(Node *root);
    Node* rootNode() const;
protected:
    QModelIndex indexForNode(Node *node) const;
    Node* nodeForIndex(const QModelIndex &index) const;
    int rowForNode(Node *node) const;
protected:
    Node *m_root;
};

#endif // READONLYMODEL_H
