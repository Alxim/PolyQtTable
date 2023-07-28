#pragma once

#include <QString>
#include <QObject>

class PropertyAbstact;
class PolyQtTableWidget;


class ObjectQtAbstract : public QObject
{
    Q_OBJECT

public:
    ObjectQtAbstract(PolyQtTableWidget* parent = nullptr) {};

    virtual QString className() {return "PrimitiveAbstract";};

    virtual bool create() = 0;

    virtual bool pasteClipboars(QModelIndex index) = 0;

    virtual QVector<PropertyAbstact*>& propVector() = 0;

    virtual bool removeRows(int row, int count, const QModelIndex& index) = 0;
};
