#include "stdafx.h"
#include "PropertyValueInt.h"
#include <QDebug>

PropertyValueInt::PropertyValueInt(QString prop_name, int default_value)
    : _name(prop_name), _value_int(default_value)
{

}

QString PropertyValueInt::name()
{
    return _name;
}

QVariant PropertyValueInt::value()
{
    return QVariant(_value_int);
}

bool PropertyValueInt::setValue(QVariant value)
{
    bool ok;
    int new_value = value.toInt(&ok);

    if ( ! ok)
        return false;

    _value_int  = new_value;
    emit valueChanched();
    return true;
}

QString PropertyValueInt::className()
{
    return QString ("PropertyValueInt");
}
