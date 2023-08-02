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

bool PropertyAbstact::invertColor()
{
    return false;
}

QRegExp PropertyAbstact::regExpToValidation() const
{
    return QRegExp();
}

QString PropertyAbstact::className()
{
    return QString("AbstactProperty");
}

bool PropertyAbstact::neetChangeDelegat()
{
    return false;
}

QWidget* PropertyAbstact::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    return nullptr;
}


