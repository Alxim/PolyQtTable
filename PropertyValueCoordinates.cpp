#include "stdafx.h"
#include "PropertyValueCoordinates.h"
#include <QDebug>


PropertyValueCoordinates::PropertyValueCoordinates(QString prop_name, QString default_value)
    : _name(prop_name), _value(default_value)
{

}

QString PropertyValueCoordinates::name()
{
    return _name;
}

QVariant PropertyValueCoordinates::value()
{
    return QVariant(_value);
}

bool PropertyValueCoordinates::setValue(QVariant value)
{
    bool ok;
    QString str = value.toString();

    ok = _reg_exp.exactMatch(str);

    if ( ! ok)
        return false;

    if(str.count(" ") == 1)
        str += " 0";

    _value  = str;
    emit valueChanched();
    return true;
}


QRegExp PropertyValueCoordinates::regExp()
{
    return _reg_exp;
}

QString PropertyValueCoordinates::className()
{
    return QString ("PropertyValueCoordinates");
}
