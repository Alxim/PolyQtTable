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
#include "PolyQtTableModel.h"
#include "ui_PolyQtTableWidget.h"


class ObjectQtAbstract;

class PolyQtTableWidget : public QWidget
{
  Q_OBJECT

public:
  PolyQtTableWidget(QWidget *parent = nullptr);
  ~PolyQtTableWidget();

private slots:
  void showDialog();
  void on_create_clicked();
  void setObjectQt();

protected:
  virtual void keyPressEvent(QKeyEvent* pe);

private:
  Ui::PolyQtTableClass ui;
  PolyQtTableModel* model = nullptr;
  ObjectQtAbstract* _object_qt;

  void clipboarsPaste();
};
