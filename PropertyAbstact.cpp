#include "stdafx.h"
#include "PropertyAbstact.h"


PropertyAbstact::PropertyAbstact()
{

}

PropertyAbstact::~PropertyAbstact()
{}


bool PropertyAbstact::flagCol(int col)
{
    switch (col) {
    case 0:
        return false;
    case 1:
        return true;
    default:
        break;
    }

    return false;
}


QString PropertyAbstact::className()
{
    return QString("PropertyAbstact");
}


bool PropertyAbstact::neetChangeDelegat()
{
    return false;
}


QWidget* PropertyAbstact::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    return nullptr;
}


