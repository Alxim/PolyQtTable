//
// Копирайт (С) 2019, ООО «Нанософт разработка». Все права защищены.
// 
// Данное программное обеспечение, все исключительные права на него, его
// документация и сопроводительные материалы принадлежат ООО «Нанософт разработка».
// Данное программное обеспечение может использоваться при разработке и входить
// в состав разработанных программных продуктов при соблюдении условий
// использования, оговоренных в «Лицензионном договоре присоединения
// на использование программы для ЭВМ «Платформа nanoCAD»».
// 
// Данное программное обеспечение защищено в соответствии с законодательством
// Российской Федерации об интеллектуальной собственности и международными
// правовыми актами.
// 
// Используя данное программное обеспечение,  его документацию и
// сопроводительные материалы вы соглашаетесь с условиями использования,
// указанными выше. 
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

  PolyQtTableModel* getModel();

  void updateTable();


private slots:
  void showDialog();
  void on_create_clicked();
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
