#include "stdafx.h"
#include "PropertyValueDouble.h"



PropertyValueDouble::PropertyValueDouble(QString prop_name, double default_value)
    : _name(prop_name), _value_double(default_value)
{

}

QString PropertyValueDouble::name()
{
    return _name;
}

QVariant PropertyValueDouble::value()
{
    return QVariant(_value_double);
}

bool PropertyValueDouble::setValue(QVariant value)
{
    bool ok;
    int new_value = value.toDouble(&ok);

    if ( ! ok)
        return false;

    _value_double  = new_value;
    emit valueChanched();
    return true;
}

QString PropertyValueDouble::className()
{
    return QString ("PropertyValueDouble");
}
