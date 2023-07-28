//
// �������� (�) 2019, ��� ��������� ����������. ��� ����� ��������.
// 
// ������ ����������� �����������, ��� �������������� ����� �� ����, ���
// ������������ � ���������������� ��������� ����������� ��� ��������� ����������.
// ������ ����������� ����������� ����� �������������� ��� ���������� � �������
// � ������ ������������� ����������� ��������� ��� ���������� �������
// �������������, ����������� � ������������� �������� �������������
// �� ������������� ��������� ��� ��� ���������� nanoCAD��.
// 
// ������ ����������� ����������� �������� � ������������ � �����������������
// ���������� ��������� �� ���������������� ������������� � ��������������
// ��������� ������.
// 
// ��������� ������ ����������� �����������,  ��� ������������ �
// ���������������� ��������� �� ������������ � ��������� �������������,
// ���������� ����. 
//

#pragma once

#include <QMainWindow>
#include "PrimitivePolyLine.h"
#include "PolyQtTableModel.h"
#include "PolyQtTableDelegat.h"
#include "PrimitiveAbstract.h"
#include "ui_PolyQtTable.h"


class PropertyAbstact;

class PolyQtTableWidget : public QWidget
{
  Q_OBJECT

public:
  PolyQtTableWidget(QWidget *parent = nullptr);
  ~PolyQtTableWidget();

private slots:
  void showDialog();
  void on_create_clicked();

protected:
  virtual void keyPressEvent(QKeyEvent* pe);

private:
  Ui::PolyQtTableClass ui;

  PolyQtTableModel* model = nullptr;


  QVector<PropertyAbstact*> prop_vector;

  PrimitivePolyLine* _primitiv;

  void clipboarsPaste();
};
