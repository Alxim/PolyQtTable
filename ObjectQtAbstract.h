#pragma once

#include <QString>
#include <QObject>
#include <QHash>

class PropertyAbstact;
class PolyQtTableWidget;
class QModelIndex;
class QTableView;


class ObjectQtAbstract : public QObject
{
    Q_OBJECT

public:
    ObjectQtAbstract(PolyQtTableWidget* parent = nullptr) {};

    static QHash <QString, ObjectQtAbstract*> OBJECT_QT_HASH;

    virtual bool create() = 0;

    virtual bool mayPasteClipboars(const QModelIndex& index);

    virtual bool pasteClipboars(QTableView* table) = 0;

    virtual QVector<PropertyAbstact*>& propVector() = 0;

    virtual bool removeRows(int row, int count, const QModelIndex& index) = 0;

    virtual bool setNanoCadObject(AcDbEntity* obj) = 0;

    virtual bool update(AcDbEntity* pEnt) = 0;

    virtual void delNanoObject(NcDbObjectId objId) = 0;
};


