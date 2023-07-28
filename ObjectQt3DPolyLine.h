#pragma once

#include <QString>
#include <QObject>

#include "ObjectQtAbstract.h"


class PropertyAbstact;
class PolyQtTableModel;
class PropertyValueInt;
class PropertyGroup;

class ObjectQt3DPolyLine : public ObjectQtAbstract
{
    Q_OBJECT

public:
    ObjectQt3DPolyLine(PolyQtTableWidget* parent = nullptr);

    QString className() override;

    bool create() override;

    bool pasteClipboars(QModelIndex index) override;

    QVector<PropertyAbstact*>& propVector()  override;

    bool removeRows(int row, int count, const QModelIndex& index) override;

//public slots:
//    void resizeVector();


private:
    QVector<PropertyAbstact*> _prop_vector;
    PropertyValueInt* _vertex_count = nullptr;
    PropertyGroup* _vertex_ptr = nullptr;
    PolyQtTableWidget* q_ptr = nullptr;

    const QString _vertex_group_name = "Вершины";
};
