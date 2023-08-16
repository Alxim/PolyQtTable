#pragma once

#include <QString>
#include <QObject>

#include "ObjectQtAbstract.h"

struct Point3D
{
    double x = 0, y = 0, z = 0;
};

class PropertyAbstact;
class PolyQtTableModel;
class PropertyValueInt;
class PropertyGroup;
class PropertyValueCoordinates;
class AcDbEntity;
class NcDbObjectId;

class ObjectQt3DPolyLine : public ObjectQtAbstract
{
    Q_OBJECT

public:
    ObjectQt3DPolyLine(PolyQtTableWidget* parent = nullptr);

    bool create() override;

    bool mayPasteClipboars(const QModelIndex& index) override;

    bool pasteClipboars(QTableView* table) override;

    QVector<PropertyAbstact*>& propVector()  override;

    bool removeRows(int row, int count, const QModelIndex& index) override;

    bool setNanoCadObject(AcDbEntity* pEnt) override;

    bool update(AcDbEntity* pEnt) override;
    
    void delNanoObject(NcDbObjectId objId) override;


public slots:
    void resizeVector();


private:
    QVector<PropertyAbstact*> _prop_vector;
    PropertyValueInt* _vertex_count = nullptr;
    /*!
     * \brief _vertex_ptr - индекс на первый элемент с координатами
     */
    PropertyValueCoordinates* _vertex_ptr = nullptr;
    PolyQtTableWidget* q_ptr = nullptr;

    const QString _vertex_group_name = "Вершины";

    bool splitStringToPoint3d(QString str, Point3D& point);
    
};
