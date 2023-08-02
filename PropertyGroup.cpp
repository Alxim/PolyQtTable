#include "stdafx.h"
#include "PropertyGroup.h"


#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>


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


void PropertyGroup::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index,
    QStyleOptionViewItem& opt)
{
   painter->fillRect(option.rect, Qt::black);
   opt.palette.setColor(QPalette::Text, Qt::white);
}
