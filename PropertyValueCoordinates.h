#ifndef PropertyValueCoordinates_H
#define PropertyValueCoordinates_H

#include "PropertyAbstact.h"
#include <QRegExp>


class PropertyValueCoordinates : public PropertyAbstact
{
public:
    PropertyValueCoordinates(QString prop_name, QString default_value);

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
     * \brief regExp - Возвращает регулярное выражение для подстановки в маску ввода и проверки вставки в модель
     * \return
     */
    virtual QRegExp regExp();

    /*!
     * \brief className получение имени класса
     * \return строка с именем класса
     */
    QString className() override;

private:
    QString _name;

    QString _value;
    const QRegExp _reg_exp = QRegExp (R"d((-{0,1}\d+ -{0,1}\d+)|(-{0,1}\d+ -{0,1}\d+ -{0,1}\d+))d");
};

#endif // PropertyValueCoordinates_H
