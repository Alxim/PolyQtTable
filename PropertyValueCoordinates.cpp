#include "stdafx.h"
#include "PropertyValueCoordinates.h"
#include <QDebug>
#include <QRegExpValidator>
#include <QModelIndex>


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


QRegExp PropertyValueCoordinates::regExpToValidation()
{
    return _reg_exp;
}

QString PropertyValueCoordinates::className()
{
    return QString ("PropertyValueCoordinates");
}


bool PropertyValueCoordinates::neetChangeDelegat()
{
    return true;
}

QWidget* PropertyValueCoordinates::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    QLineEdit* le = new QLineEdit(parent);

    QRegExpValidator* val = new QRegExpValidator(_reg_exp);
    le->setValidator(val);
    return le;
}

void PropertyValueCoordinates::setEditorData(QWidget* editor, const QModelIndex& index)
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit* le = static_cast<QLineEdit*>(editor);
    le->setText(value);
}

void PropertyValueCoordinates::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)
{
    QLineEdit* le = static_cast<QLineEdit*>(editor);
    QString value = le->text();
    model->setData(index, value, Qt::EditRole);
}
