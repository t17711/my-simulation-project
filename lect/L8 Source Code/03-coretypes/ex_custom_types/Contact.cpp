#include "Contact.h"

Contact::Contact()
{
}

void Contact::setName(const QString &name)
{
    if ( m_name != name )
        m_name = name;
}

QString Contact::name() const
{
    return m_name;
}
