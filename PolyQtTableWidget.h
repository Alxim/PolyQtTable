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

  PolyQtTableModel* getModel();

  void updateTable();


private slots:
  void showDialog();
  void on_create_clicked();
  void on_updateObject_clicked();
  void setObjectQt();
  void customMenu(QPoint pos);

protected:
    void keyPressEvent(QKeyEvent* pe);
	bool eventFilter(QObject* obj, QEvent* event);

private:
  Ui::PolyQtTableClass ui;
  PolyQtTableModel* _model = nullptr;
  ObjectQtAbstract* _object_qt;

  

  void clipboarsPaste();
};
