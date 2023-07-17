#include "stdafx.h"
#include "PropertyGroup.h"

PropertyGroup::PropertyGroup(QString group_name)
    : _name(group_name)
{

}

bool PropertyGroup::flagCol(int /*col*/)
{
    return false;
}

QString PropertyGroup::name()
{
    return _name;
}

QVariant PropertyGroup::value()
{
    return _value;
}

bool PropertyGroup::setValue(QVariant /*value*/)
{
    return false;
}

bool PropertyGroup::invertColor()
{
    return true;
}

QString PropertyGroup::className()
{
    return QString ("PropertyGroup");
}
