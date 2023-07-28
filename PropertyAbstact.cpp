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

QRegExp PropertyAbstact::regExpToValidation()
{
    return QRegExp();
}

QString PropertyAbstact::className()
{
    return QString("AbstactProperty");
}


