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

    bool create() override;

    bool pasteClipboars(QModelIndex index) override;

public slots:
    void resizeVector();


private:
    QVector<PropertyAbstact*>* _prop_vector = nullptr;
    PropertyValueInt* _vertex_count = nullptr;
    PolyQtTableModel * _model = nullptr;

    const int _point_start_index = 4;
};
