#pragma once
#include <qabstractitemmodel.h>


#include <QObject>
#include <QModelIndex>
#include <QStyledItemDelegate>
#include <QItemDelegate>
#include <QVector>

#include "PropertyAbstact.h"


class PolyQtTableModel : public QAbstractTableModel
{
public:
    PolyQtTableModel(QVector<PropertyAbstact*>* prop_vector);

    QVariant data(const QModelIndex& index, int nRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int nRole = Qt::EditRole) override;
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex&) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int nSection, Qt::Orientation orientation, int nRole) const override;

    bool insertRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& index = QModelIndex()) override;

    QRegExp regExp(const QModelIndex& index) const;

private:
    QVector<PropertyAbstact*>* _prop_vector;
};

