#ifndef POLYQTTABLEDELEGAT_H
#define POLYQTTABLEDELEGAT_H

#include <QItemDelegate>

class PolyQtTableDelegat : public QItemDelegate
{
public:
    explicit PolyQtTableDelegat(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem& option,
               const QModelIndex& index ) const;
};

#endif // POLYQTTABLEDELEGAT_H
