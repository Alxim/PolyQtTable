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

#include "stdafx.h"
#include "PolyQtTable.h"
#include "PrimitivePolyLine.h"

#include <qdebug.h>
#include <QMessageBox>
#include <QKeyEvent>

PolyQtTableWidget::PolyQtTableWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.create, SIGNAL(clicked()), this, SLOT(showDialog()));

	_primitiv = new PrimitivePolyLine(&prop_vector);
	primitives.append(_primitiv);

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

	acutPrintf(L"\nHello, NRX!\n");
	QString str = "Test";
	QMessageBox::about(this, "QMessageBox",
		QString("Hello, this is %1 signals and slots!").arg(str)
	);
}

void PolyQtTableWidget::on_create_clicked()
{
	//primitives.at(ui.comboBox->currentIndex())->resizeVector();
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
