#pragma once

#include <QString>
#include <QObject>

#include "PrimitiveAbstract.h"


class PropertyAbstact;
class PolyQtTableModel;
class PropertyValueInt;

class PrimitivePolyLine : public PrimitiveAbstract
{
    Q_OBJECT

public:
    PrimitivePolyLine(QVector<PropertyAbstact*>* prop_vector);

    void setModel(PolyQtTableModel* model) override;

    QString className() override;


public slots:
    void resizeVector();


private:
    QVector<PropertyAbstact*>* _prop_vector = nullptr;
    PropertyValueInt* _vertex_count = nullptr;
    PolyQtTableModel * _model = nullptr;
};
