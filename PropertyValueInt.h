#ifndef PROPERTYVALUEINT_H
#define PROPERTYVALUEINT_H

#include "PropertyAbstact.h"

class PropertyValueInt : public PropertyAbstact
{
public:
    PropertyValueInt(QString prop_name, int default_value);

    /*!
     * \brief name - получить имя свойства
     * \return строка с именем класса
     */
    QString name() override;

    /*!
     * \brief value - получить значение находящееся в свойстве
     * \return может быть различных типо, поэтому преобразуется в QVariant
     */
    QVariant value() override;


    /*!
     * \brief setValue - установить значение свойств, в данном классе простую строку.
     * \param value - значение полученное из делегата и передаваемое в модель
     */
    bool setValue(QVariant value) override;


    /*!
     * \brief setMaximum - установить минимальное значение для числа
     * \param value - значение полученное из делегата и передаваемое в модель
     */
    bool setMaximum(int min);

    /*!
     * \brief setMinimun - установить минимальное значение для числа
     * \param value - значение полученное из делегата и передаваемое в модель
     */
    bool setMinimun(int min);

    /*!
     * \brief className получение имени класса
     * \return строка с именем класса
     */
    QString className() override;


    bool neetChangeDelegat() override;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
        const QModelIndex& index) override;

    void setEditorData(QWidget* editor, const QModelIndex& index) override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
        const QModelIndex& index) override;


private:
    QString _name;

    int _value_int;
    int _max = 999999;
    int _min = 0;
};

#endif // PROPERTYVALUEINT_H
