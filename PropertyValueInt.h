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
     * \brief className получение имени класса
     * \return строка с именем класса
     */
    QString className() override;

private:
    QString _name;

    int _value_int;
};

#endif // PROPERTYVALUEINT_H
