#include "stdafx.h"
#include "ObjectQt3DPolyLine.h"
#include "PolyQtTableDelegat.h"
#include "PolyQtTableWidget.h"
#include "PropertyAbstact.h"

#include <qdebug.h>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMenu>


void createNcEditorReactor();
void clearNcEditorReactor();

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

	ui.tableView->installEventFilter(this);

	ui.updateObject->hide();

	//  Установка контекстного меню
	ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenu(QPoint)));
}

PolyQtTableWidget::~PolyQtTableWidget()
{
}

PolyQtTableModel* PolyQtTableWidget::getModel()
{
	return _model;
}

void PolyQtTableWidget::updateTable()
{
	ui.tableView->selectAll();
	ui.tableView->clearSelection();
}

bool PolyQtTableWidget::setNanoCadObject(AcDbEntity* pEnt)
{
	std::string str = CT2A(pEnt->isA()->name());

	ObjectQtAbstract* temp = ObjectQtAbstract::OBJECT_QT_HASH.value(QString::fromStdString(str), nullptr);

	if (temp != nullptr)
	{
		acutPrintf(TEXT("\n temp != nullptr"));

		if (temp->setNanoCadObject(pEnt))
		{
			ui.updateObject->show();
			_nano_ent = pEnt;
			return true;
		}		
	}

	acutPrintf(TEXT("\n temp == nullptr \n %d "), ObjectQtAbstract::OBJECT_QT_HASH.size());
	ui.updateObject->hide();

	return false;
}


void PolyQtTableWidget::setObjectQt()
{
	_object_qt = new ObjectQt3DPolyLine(this);
	_model = new PolyQtTableModel( _object_qt );
}

void PolyQtTableWidget::customMenu(QPoint pos)
{
	QMenu* menu = new QMenu(this);

	if (_object_qt->mayPasteClipboars(ui.tableView->currentIndex() ) )
	{
		QAction* insert = new QAction("Вставить", this);

		connect(insert, &QAction::triggered, this, &PolyQtTableWidget::clipboarsPaste);

		menu->addAction(insert);
	}
	menu->popup(ui.tableView->viewport()->mapToGlobal(pos));
}

void PolyQtTableWidget::showDialog()
{
	_object_qt->create();
}

void PolyQtTableWidget::on_create_clicked()
{
	
}

void PolyQtTableWidget::on_updateObject_clicked()
{
	_object_qt->update(_nano_ent);
}


void PolyQtTableWidget::on_deleteObject_clicked()
{
	_object_qt->delNanoObject(_nano_ent->objectId());
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


bool PolyQtTableWidget::eventFilter(QObject* obj, QEvent* event)
{
    if ((event->type() != QEvent::KeyPress))
        return false;

    QKeyEvent* key_event = static_cast<QKeyEvent*>(event);

	switch (key_event->key())
	{
	case Qt::Key_V:
		if (key_event->modifiers() == Qt::ControlModifier)
		{
			clipboarsPaste();
		}
		break;

	case Qt::Key_Insert:
		if (key_event->modifiers() == Qt::ShiftModifier)
		{
			clipboarsPaste();
		}
		break;

	default:
		;
	}

    return false;
}

void PolyQtTableWidget::showEvent(QShowEvent* event)
{
	acutPrintf(TEXT("\nshowEvent - createNcEditorReactor"));
	createNcEditorReactor();
}

void PolyQtTableWidget::hideEvent(QHideEvent* event)
{
	acutPrintf(TEXT("\nhideEvent - clearNcEditorReactor"));
	clearNcEditorReactor();
}

void PolyQtTableWidget::closeEvent(QCloseEvent* event)
{
	acutPrintf(TEXT("\ncloseEvent - clearNcEditorReactor"));
	clearNcEditorReactor();
}


void PolyQtTableWidget::clipboarsPaste()
{
	_object_qt->pasteClipboars(ui.tableView);
}
