#include "stdafx.h"
#include "PolyQtTableModel.h"




PolyQtTableModel::PolyQtTableModel(QVector<PropertyAbstact*>* prop_vector)
    : _prop_vector(prop_vector)
{
}

QVariant PolyQtTableModel::data(const QModelIndex& index, int nRole) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    PropertyAbstact* prop = _prop_vector->at(row);

    switch (index.column())
    {
    case 0:
        return (nRole == Qt::DisplayRole)
            ? QVariant(prop->name())
            : QVariant();
    case 1:
        return (nRole == Qt::DisplayRole || nRole == Qt::EditRole)
            ? QVariant(prop->value())
            : QVariant();
    case 2:
        return (nRole == Qt::DisplayRole || nRole == Qt::EditRole)
            ? QVariant(prop->className())
            : QVariant();
    }

    return QVariant();
}



bool PolyQtTableModel::setData(const QModelIndex& index, const QVariant& value, int nRole)
{
    int row = index.row();

    if (index.isValid() && nRole == Qt::EditRole)
    {
        _prop_vector->at(row)->setValue(value);
        return true;
    }

    return false;
}


int PolyQtTableModel::rowCount(const QModelIndex&) const
{
    return _prop_vector->size();
}


int PolyQtTableModel::columnCount(const QModelIndex&) const
{
    return 3;
}


Qt::ItemFlags PolyQtTableModel::flags(const QModelIndex& index) const
{
    int row = index.row(),
        col = index.column();

    Qt::ItemFlags ret = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (_prop_vector->at(row)->flagCol(col))
        ret |= Qt::ItemIsEditable;

    return ret;
}



QVariant PolyQtTableModel::headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const
{
    if (nRole != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (nSection)
        {
        case 0:
            return QString("Свойство");
        case 1:
            return QString("Значение");
        case 2:
            return QString("Тип");
        }
    }

    return QVariant();//QString::number(nSection + 1);
}

bool PolyQtTableModel::insertRows(int position, int rows, const QModelIndex& index)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    endInsertRows();
    return true;
}



bool PolyQtTableModel::removeRows(int row, int count, const QModelIndex& index)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int _row = 0; _row < count; _row++)
        _prop_vector->removeAt(row);

    endRemoveRows();
    return true;
}

QRegExp PolyQtTableModel::regExpToValidation(const QModelIndex &index) const
{
    return _prop_vector->at(index.row())->regExpToValidation();
}
