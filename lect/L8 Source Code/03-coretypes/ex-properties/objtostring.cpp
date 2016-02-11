/*************************************************************************
 *
 * Copyright (c) 2012, Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QtGui>
#include <QtWidgets/QApplication>

/** A function which converts any QObject with
    properties into a QString, using metaproperties.
*/
//start id=objToString
QString objToString(const QObject* obj) {
    QStringList result;
    const QMetaObject* meta = obj->metaObject();            /* We introspect
        into the object via the QMetaObject. */
    result += QString("class %1 : public %2 {").arg(meta->className())
        .arg(meta->superClass()->className());
    for (int i=0; i < meta->propertyCount(); ++i) {
        const QMetaProperty qmp = meta->property(i);
        QVariant value = obj->property(qmp.name());
        if (value.canConvert(QVariant::String))
            result += QString("  %1 %2 = %3;")
            .arg(qmp.typeName())
            .arg(qmp.name())
            .arg(value.toString());
    }
    result += "};";
    return result.join("\n");
}
int main (int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("digia");
    app.setApplicationName("testproperties");
    app.setOrganizationDomain("com");
    qDebug() << objToString(&app);
}

