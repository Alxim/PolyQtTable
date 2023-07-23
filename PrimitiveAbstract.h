#pragma once

#include <QString>
#include <QObject>

class PropertyAbstact;
class PolyQtTableModel;


class PrimitiveAbstract : public QObject
{
    Q_OBJECT

public:
    PrimitiveAbstract(QVector<PropertyAbstact*>* prop_vector);

    virtual void setModel(PolyQtTableModel* model);

    virtual QString className();

    virtual bool create() = 0;

    virtual bool pasteClipboars(QModelIndex index) = 0;

};
