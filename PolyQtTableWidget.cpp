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
#include "ObjectQt3DPolyLine.h"
#include "PolyQtTableDelegat.h"
#include "PolyQtTableWidget.h"
#include "PropertyAbstact.h"

#include <qdebug.h>
#include <QMessageBox>
#include <QKeyEvent>

PolyQtTableWidget::PolyQtTableWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.create, SIGNAL(clicked()), this, SLOT(showDialog()));

	setObjectQt();

	ui.tableView->setModel(_model);

	ui.tableView->resizeColumnsToContents();
	ui.tableView->horizontalHeader()->hide();
	ui.tableView->verticalHeader()->hide();


	ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.tableView->setItemDelegate(new PolyQtTableDelegat());
}

PolyQtTableWidget::~PolyQtTableWidget()
{}

PolyQtTableModel* PolyQtTableWidget::getModel()
{
	return _model;
}


void PolyQtTableWidget::setObjectQt()
{
	_object_qt = new ObjectQt3DPolyLine(this);
	_model = new PolyQtTableModel( _object_qt );
}

void PolyQtTableWidget::showDialog()
{
	qDebug() << "Test print";
	_object_qt->create();

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
	_object_qt->pasteClipboars(index);
	ui.tableView->setCurrentIndex(index);
}
