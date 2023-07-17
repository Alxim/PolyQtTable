#include "stdafx.h"
#include "PrimitivePolyLine.h"
#include "PropertyGroup.h"
#include "PropertyValueInt.h"
#include "PropertyValueDouble.h"
#include "PolyQtTableModel.h"
#include "PropertyValueCoordinates.h"

#include <QDebug>

PrimitivePolyLine::PrimitivePolyLine(QVector<PropertyAbstact*> *prop_vector)
    : _prop_vector(prop_vector), PrimitiveAbstract(prop_vector)
{
    _prop_vector->clear();

    PropertyGroup* pg_1 = new PropertyGroup("Общие");
    _prop_vector->append(pg_1);

    _vertex_count = new PropertyValueInt("Количество вершин", 2);
    _prop_vector->append(_vertex_count);

    PropertyValueInt* pvi_2 = new PropertyValueInt("Точность", 2);
    _prop_vector->append(pvi_2);

    PropertyGroup* pg_2 = new PropertyGroup("Вершины");
    _prop_vector->append(pg_2);

    resizeVector();

    connect(_vertex_count, SIGNAL(valueChanched()), this, SLOT(resizeVector()));
}

void PrimitivePolyLine::setModel(PolyQtTableModel *model)
{
    _model = model;
}

void PrimitivePolyLine::resizeVector()
{
    int curr_count = _prop_vector->count(),
        new_count = _vertex_count->value().toInt() + 4;


    if(curr_count == new_count)
        return;

    if(curr_count < new_count)
    {
        for(int i = curr_count; i < new_count; i++)
        {
            _prop_vector->append(new PropertyValueCoordinates(QString("Вершина %1").arg(i - 3), "0 0 0"));
            if(_model != nullptr)
                _model->insertRows(_model->rowCount(), 1);
        }
    }

    if(curr_count > new_count)
    {
        while(curr_count != new_count)
        {
            if(_model != nullptr)
                _model->removeRows(_model->rowCount()-1, 1);

            curr_count--;
        }
    }

}


QString PrimitivePolyLine::className()
{
    return "PrimitivePolyLine";
}
