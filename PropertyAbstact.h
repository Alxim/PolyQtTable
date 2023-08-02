#pragma once

#include <QString>
#include <QVariant>

class QStyleOptionViewItem;
class QAbstractItemModel;

class PropertyAbstact : public QObject
{
    Q_OBJECT

public:
    PropertyAbstact();
    ~PropertyAbstact();
    /*!
    * \brief flagCol  Разрешение на редактирование колонки окна свойств
    * \param col  номер колонки для проверки
    * \return  в абстрактном классе по умолчанию изменять можно только вторую колонку
    */
    virtual bool flagCol(int col);

    /*!
     * \brief name - получить имя свойства
     * \return строка с именем класса
     */
    virtual QString name() = 0;

    /*!
     * \brief value - получить значение находящееся в свойстве
     * \return может быть различных типо, поэтому преобразуется в QVariant
     */
    virtual QVariant value() = 0;


    /*!
     * \brief setValue - установить значение свойств
     * \param value - значение полученное из делегата и передаваемое в модель
     */
    virtual bool setValue(QVariant value) = 0;


    /*!
     * \brief invertColor необходимо инвертировац цвет фона и шрифта
     * \return инвентровать надо только для заголовка
     */
    virtual bool invertColor();


    /*!
     * \brief regExpToValidation - Возвращает регулярное выражение для подстановки в маску ввода и проверки вставки в модель
     * \return
     */
    virtual QRegExp regExpToValidation() const;


    /*!
     * \brief className получение имени класса
     * \return строка с именем класса
     */
    virtual QString className();

    virtual bool neetChangeDelegat();

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
        const QModelIndex& index);

    virtual void setEditorData(QWidget* editor, const QModelIndex& index) {};
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model,
        const QModelIndex& index) {};

    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index, QStyleOptionViewItem& opt) {};

signals:
    void valueChanched();
};
