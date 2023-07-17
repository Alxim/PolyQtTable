#include "stdafx.h"
#include "PrimitiveAbstract.h"
#include "PropertyGroup.h"
#include "PropertyValueInt.h"
#include "PropertyValueDouble.h"
#include "PolyQtTableModel.h"


PrimitiveAbstract::PrimitiveAbstract(QVector<PropertyAbstact*> *prop_vector)
    //: _prop_vector(prop_vector)
{
    /*_prop_vector->clear();

    PropertyGroup* pg_1 = new PropertyGroup("Общие");
    _prop_vector->append(pg_1);

    PropertyValueInt* pvi_2 = new PropertyValueInt("Точность", 2);
    _prop_vector->append(pvi_2);

    PropertyGroup* pg_2 = new PropertyGroup("Вершины");
    _prop_vector->append(pg_2);
    */

}

void PrimitiveAbstract::setModel(PolyQtTableModel *model)
{
//    _model = model;
}

QString PrimitiveAbstract::className()
{
    return "PrimitiveAbstract";
}
