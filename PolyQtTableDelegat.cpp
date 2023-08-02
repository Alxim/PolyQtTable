#include "stdafx.h"
#include "PolyQtTableDelegat.h"
#include "PolyQtTableModel.h"

#include <QPainter>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

PolyQtTableDelegat::PolyQtTableDelegat(QObject *parent)
    : QItemDelegate{parent}
{

}


QWidget *PolyQtTableDelegat::createEditor(QWidget *parent, const QStyleOptionViewItem & option ,
                                          const QModelIndex & index) const
{
    const PolyQtTableModel* model = static_cast<const PolyQtTableModel*> (index.model());
    PropertyAbstact* prop = model->getProperty(index);

    if (prop->neetChangeDelegat())
    {
        return prop->createEditor(parent, option, index);
    }

    return QItemDelegate::createEditor(parent, option, index);
}

//***********************************************************************************************************************************

void PolyQtTableDelegat::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const PolyQtTableModel* model = static_cast<const PolyQtTableModel*> (index.model());
    PropertyAbstact* prop = model->getProperty(index);

    if (prop->neetChangeDelegat())
        prop->setEditorData(editor, index);
    else
        QItemDelegate::setEditorData(editor, index);
}

//***********************************************************************************************************************************

void PolyQtTableDelegat::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const PolyQtTableModel* model_poly = static_cast<const PolyQtTableModel*> (index.model());
    PropertyAbstact* prop = model_poly->getProperty(index);

    if (prop->neetChangeDelegat())
        prop->setModelData(editor, model, index);
    else
        QItemDelegate::setModelData(editor, model, index);
}

//***********************************************************************************************************************************

void PolyQtTableDelegat::updateEditorGeometry(QWidget *editor,
                                              const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


void PolyQtTableDelegat::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt;
    
    opt = setOptions(index, option);


    const PolyQtTableModel* model_poly = static_cast<const PolyQtTableModel*> (index.model());
    PropertyAbstact* prop = model_poly->getProperty(index);

    prop->paint(painter, option, index, opt);


    drawDisplay(painter, opt, opt.rect, index.data().toString());
}
