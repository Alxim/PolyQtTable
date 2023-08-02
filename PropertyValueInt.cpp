#include "stdafx.h"
#include "PropertyValueInt.h"
#include <QDebug>
#include <QSpinBox>
#include <QModelIndex>


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

bool PropertyValueInt::setMaximum(int man)
{
    if (man > _min)
    {
        _max = man;
        return true;
    }

    return false;
}

bool PropertyValueInt::setMinimun(int min)
{
    if (min < _max)
    {
        _min = min;
        return true;
    }

    return false;
}

QString PropertyValueInt::className()
{
    return QString ("PropertyValueInt");
}

bool PropertyValueInt::neetChangeDelegat()
{
    return true;
}

QWidget* PropertyValueInt::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    QSpinBox* spin = new QSpinBox(parent);
    spin->setMinimum(2);
    spin->setMaximum(2147483647);
    return spin;
}

void PropertyValueInt::setEditorData(QWidget* editor, const QModelIndex& index)
{
    bool ok;
    int value = index.model()->data(index, Qt::EditRole).toInt(&ok);

    if (!ok)
        return;

    QSpinBox* sb = static_cast<QSpinBox*>(editor);
    sb->setValue(value);
}

void PropertyValueInt::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)
{
    QSpinBox* sb = static_cast<QSpinBox*>(editor);
    int value = sb->value();
    model->setData(index, value, Qt::EditRole);
    emit valueChanched();
}

