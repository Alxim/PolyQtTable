#pragma once

#include <QString>
#include <QObject>

#include "ObjectQtAbstract.h"


class PropertyAbstact;
class PolyQtTableModel;
class PropertyValueInt;
class PropertyGroup;
class PropertyValueCoordinates;
class AcDbEntity;

class ObjectQt3DPolyLine : public ObjectQtAbstract
{
    Q_OBJECT

public:
    ObjectQt3DPolyLine(PolyQtTableWidget* parent = nullptr);

    bool create() override;

    bool pasteClipboars(QModelIndex index) override;

    QVector<PropertyAbstact*>& propVector()  override;

    bool removeRows(int row, int count, const QModelIndex& index) override;

    bool setNanoCadObject(AcDbEntity* pEnt) override;

public slots:
    void resizeVector();


private:
    QVector<PropertyAbstact*> _prop_vector;
    PropertyValueInt* _vertex_count = nullptr;
    PropertyValueCoordinates* _vertex_ptr = nullptr;
    PolyQtTableWidget* q_ptr = nullptr;

    const QString _vertex_group_name = "Вершины";
};
