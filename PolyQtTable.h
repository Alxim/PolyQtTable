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
