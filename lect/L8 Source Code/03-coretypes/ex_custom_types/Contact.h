#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QMetaType>

class Contact
{
public:
    Contact();
    void setName( const QString& name );
    QString name() const;

private:
    QString m_name;
};

Q_DECLARE_METATYPE(Contact);

#endif // CONTACT_H
