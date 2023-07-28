#pragma once

#include <QString>
#include <QVariant>

class PropertyAbstact : public QObject
{
    Q_OBJECT

public:
    PropertyAbstact();
    ~PropertyAbstact();
    /*!
    * \brief flagCol  ���������� �� �������������� ������� ���� �������
    * \param col  ����� ������� ��� ��������
    * \return  � ����������� ������ �� ��������� �������� ����� ������ ������ �������
    */
    virtual bool flagCol(int col);

    /*!
     * \brief name - �������� ��� ��������
     * \return ������ � ������ ������
     */
    virtual QString name() = 0;

    /*!
     * \brief value - �������� �������� ����������� � ��������
     * \return ����� ���� ��������� ����, ������� ������������� � QVariant
     */
    virtual QVariant value() = 0;


    /*!
     * \brief setValue - ���������� �������� �������
     * \param value - �������� ���������� �� �������� � ������������ � ������
     */
    virtual bool setValue(QVariant value) = 0;


    /*!
     * \brief invertColor ���������� ������������ ���� ���� � ������
     * \return ������������ ���� ������ ��� ���������
     */
    virtual bool invertColor();


    /*!
     * \brief regExpToValidation - ���������� ���������� ��������� ��� ����������� � ����� ����� � �������� ������� � ������
     * \return
     */
    virtual QRegExp regExpToValidation() const;


    /*!
     * \brief className ��������� ����� ������
     * \return ������ � ������ ������
     */
    virtual QString className();

signals:
    void valueChanched();
};
