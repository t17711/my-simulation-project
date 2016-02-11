#ifndef CITYMODEL_H
#define CITYMODEL_H

#include <QtWidgets/QStandardItemModel>

#include "cityengine.h"

class CityModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CityModel(QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const;

private:
    void setupModel();
};

#endif // CITYMODEL_H
