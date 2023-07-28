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

#include "stdafx.h"
#include "PolyQtTable.h"
#include "PrimitivePolyLine.h"
#include "PropertyAbstact.h"

#include <qdebug.h>
#include <QMessageBox>
#include <QKeyEvent>

PolyQtTableWidget::PolyQtTableWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.create, SIGNAL(clicked()), this, SLOT(showDialog()));

	_primitiv = new PrimitivePolyLine(&prop_vector);
	
	model = new PolyQtTableModel(&prop_vector);
	//poly_line->setModel(model);

	ui.tableView->setModel(model);

	ui.tableView->resizeColumnsToContents();
	ui.tableView->hideColumn(2);
	ui.tableView->horizontalHeader()->hide();
	ui.tableView->verticalHeader()->hide();

	ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.tableView->setItemDelegate(new PolyQtTableDelegat());

}

PolyQtTableWidget::~PolyQtTableWidget()
{}

void PolyQtTableWidget::showDialog()
{
	qDebug() << "Test print";
	_primitiv->create();
	/**/

	acutPrintf(L"\nCreate 3Dpolyline\n");
}

void PolyQtTableWidget::on_create_clicked()
{
	
}

void PolyQtTableWidget::keyPressEvent(QKeyEvent* pe)
{
	switch (pe->key())
	{
	case Qt::Key_V:
		if (pe->modifiers() == Qt::ControlModifier)
		{
			clipboarsPaste();
		}
		break;

	case Qt::Key_Insert:
		if (pe->modifiers() == Qt::ShiftModifier)
		{
			clipboarsPaste();
		}
		break;

	default:
		;
	}

	pe->accept();
}

void PolyQtTableWidget::clipboarsPaste()
{
	QModelIndex index = ui.tableView->currentIndex();
	_primitiv->pasteClipboars(index);
	ui.tableView->setCurrentIndex(index);
}
