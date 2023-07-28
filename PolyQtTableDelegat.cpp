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
    QComboBox* cd;
    QLineEdit* le;

    switch(index.column())
    {

    case 0:
        return QItemDelegate::createEditor(parent, option, index);


    case 1:
    {
        le = new QLineEdit(parent);

        const PolyQtTableModel* model = static_cast<const PolyQtTableModel*> (index.model() );
        QRegExp re = model->regExpToValidation(index);
        if(! re.isEmpty() )
        {
            QRegExpValidator* val = new QRegExpValidator(re);
            le->setValidator(val);
        }
        return le;
    }

    default:
        return QItemDelegate::createEditor(parent, option, index);
    }
}

//***********************************************************************************************************************************

void PolyQtTableDelegat::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox* cd;
    QLineEdit* le;

    QString value;

    switch(index.column())
    {
    case 0:
        QItemDelegate::setEditorData(editor, index);

        break;

    case 1:
        value = index.model()->data(index, Qt::EditRole).toString();
        le = static_cast<QLineEdit*>(editor);
        le->setText(value);

        break;

    default:
        QItemDelegate::setEditorData(editor, index);
        break;
    }
}

//***********************************************************************************************************************************

void PolyQtTableDelegat::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* cd;
    QLineEdit* le;
    QString value;

    switch(index.column())
    {

    case 0:
        QItemDelegate::setModelData(editor, model, index);
        break;

    case 1:
        le = static_cast<QLineEdit*>(editor);
        value = le->text();
        model->setData(index, value, Qt::EditRole);
        break;


    default:
        QItemDelegate::setModelData(editor, model, index);
        break;
    }
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
    QString class_name;
    QBrush br;
    int row;


    opt = setOptions(index, option);
    row = index.row();
    class_name = index.sibling(row, 2).data().toString();

    if( class_name == "PropertyGroup")
    {
        painter->fillRect(option.rect, Qt::black);
        opt.palette.setColor(QPalette::Text, Qt::white);
    }
    else
        painter->fillRect(option.rect, Qt::white);


    drawDisplay(painter, opt, opt.rect, index.data().toString());
}
