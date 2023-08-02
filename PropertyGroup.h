#ifndef PROPERTYGROUP_H
#define PROPERTYGROUP_H

#include "PropertyAbstact.h"

class PropertyGroup : public PropertyAbstact
{
public:
    PropertyGroup(QString group_name);

    /*!
     * \brief flagCol - разрешение на редактирование второй колонки окна свойств
     * \return в класс групп свойств любые правки черз делегат запрещены
     */
    bool flagCol(int) override;

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
     * \brief invertColor необходимо инвертировац цвет фона и шрифта
     * \return инвентровать надо только для заголовка
     */
    bool invertColor() override;


    /*!
     * \brief className получение имени класса
     * \return строка с именем класса
     */
    QString className() override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index, QStyleOptionViewItem& opt) override;

private:
    QString _name;
    QVariant _value;

};

#endif // PROPERTYGROUP_H
