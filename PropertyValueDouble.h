#ifndef PROPERTYVALUEDOUBLE_H
#define PROPERTYVALUEDOUBLE_H

#include "PropertyAbstact.h"

class PropertyValueDouble : public PropertyAbstact
{
public:
    PropertyValueDouble(QString prop_name, double default_value = 0);

    /*!
     * \brief name - получить имя свойства
     * \return строка с именем класса
     */
    QString name() override;

    /*!
     * \brief value - получить значение находящееся в свойстве
     * \return может быть различных типов, поэтому преобразуется в QVariant
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

    double _value_double;
};

#endif // PROPERTYVALUEDOUBLE_H
