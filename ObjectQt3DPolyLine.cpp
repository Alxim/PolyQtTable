#include "stdafx.h"
#include "ObjectQt3DPolyLine.h"
#include "PropertyGroup.h"
#include "PropertyValueInt.h"
#include "PropertyValueDouble.h"
#include "PolyQtTableWidget.h"
#include "PropertyValueCoordinates.h"
#include "hostUI.h"
#include "hostQt.h"
#include "nc2ac.h"
#include "objptr.h"

#include <QDebug>
#include <QClipboard>
#include <QGuiApplication>

ObjectQt3DPolyLine::ObjectQt3DPolyLine(PolyQtTableWidget* parent)
	: q_ptr(parent)
{
	_prop_vector.clear();

	PropertyGroup* pg_1 = new PropertyGroup("Общие");
	_prop_vector.append(pg_1);

	_vertex_count = new PropertyValueInt("Количество вершин", 3);
	_prop_vector.append(_vertex_count);

	PropertyGroup* pg_2 = new PropertyGroup(_vertex_group_name);
	_prop_vector.append(pg_2);

	_vertex_ptr = new PropertyValueCoordinates(
		QString("Вершина 1"),
		QString("%1 %2 %3").arg(0).arg(0).arg(0));
	_prop_vector.append(_vertex_ptr);
	
	resizeVector();

	connect(_vertex_count, SIGNAL(valueChanched()), this, SLOT(resizeVector()));

	NcDb3dPolyline pEnt;
	std::string str = CT2A(pEnt.isA()->name());
	ObjectQtAbstract::OBJECT_QT_HASH.insert(QString::fromStdString(str), this);
}


void ObjectQt3DPolyLine::resizeVector()
{
	int point_start_index = _prop_vector.indexOf(_vertex_ptr);

	int curr_count = _prop_vector.count(),
		new_count = _vertex_count->value().toInt() + point_start_index;


	if (curr_count == new_count)
		return;

	PolyQtTableModel* _model = q_ptr->getModel();

	if (curr_count < new_count)
	{
		for (int i = curr_count; i < new_count; i++)
		{
			//double x = cos(3 * i) + i, y = sin(3 * i) + i, z = exp(double(i) / 10);
			double x = (pow(2, i)* (i%2) + i ) * 1000, 
				y = (pow(2, i) * ((i+1) % 2) + i) * 1000, 
				z = pow(2, i) * 1000;
			_prop_vector.append(new PropertyValueCoordinates(
				QString("Вершина %1").arg(i - point_start_index + 1), 
				QString("%1 %2 %3").arg(x).arg(y).arg(z)
			));
			if (_model != nullptr)
				_model->insertRows(_model->rowCount(), 1);

		}
	}

	if (curr_count > new_count)
	{
		while (curr_count != new_count)
		{
			_model->removeRows(_model->rowCount() - 1, 1);

			curr_count--;
		}
	}

}



bool ObjectQt3DPolyLine::create()
{
	// Создание 3Д полпелинии
	NcGePoint3dArray ptArr;

	int point_start_index = _prop_vector.indexOf(_vertex_ptr);
	int count = _prop_vector.count();
	ptArr.setLogicalLength(count - point_start_index + 1);

	for (int i = point_start_index; i < count; i++)
	{
		double x = cos(3 * i) + i, y = sin (3 * i) + i, z = exp(double(i)/10);

		QString value = _prop_vector.at(i)->value().toString();
		QStringList lst = value.split(" ");

		switch (lst.count())
		{
		case 3:
			z = lst.at(2).toDouble();
		case 2:
			x = lst.at(0).toDouble();
			y = lst.at(1).toDouble();
			break;
		default:
			break;
		}

		ptArr[i - point_start_index].set(x, y, z);
	}


	NcDb3dPolyline* pNewPline = new NcDb3dPolyline(NcDb::k3dSimplePoly, ptArr, NRX::kTrue);

	pNewPline->setColorIndex(3);

	//  polygon mesh constructor without any parameter
	NcDbPolygonMesh* pMesh = new NcDbPolygonMesh();
	pMesh->setMSize(1);
	pMesh->setNSize(4);
	pMesh->makeMClosed();
	pMesh->makeNClosed();

	NcDbVoidPtrArray arr;
	arr.append(pMesh);


	NcDbBlockTable* pBlockTable;
	ncdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, NcDb::kForRead);

	//PolyNcEditorReactor* reactor = new PolyNcEditorReactor();
	NcDbObjectReactor* reactor_obj = new NcDbObjectReactor();
	

	//  NcDb::kForRead - Замыкать фигуру, если не ставить, то будет разорванная

	NcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, NcDb::kForWrite);
	pBlockTable->close();


	NcDbObjectId plineObjId;
	pBlockTableRecord->appendAcDbEntity(plineObjId, pNewPline);


	NcDbObjectIterator* pVertIter = pNewPline->vertexIterator();
	NcDb3dPolylineVertex* pVertex;
	NcGePoint3d location;
	NcDbObjectId vertexObjId;

	for (int vertexNumber = 0; !pVertIter->done();
		vertexNumber++, pVertIter->step())
	{
		vertexObjId = pVertIter->objectId();
		ncdbOpenObject(pVertex, vertexObjId,
			NcDb::kForRead);
		location = pVertex->position();
		pVertex->close();
		NcDbPolygonMeshVertex* polyVertex = new NcDbPolygonMeshVertex(pVertex->position());
		pMesh->appendVertex(polyVertex);
		polyVertex->close();
	}
	delete pVertIter;


	pBlockTableRecord->appendAcDbEntity(pMesh);
	pBlockTableRecord->close();
	pNewPline->close();
	pMesh->close();

	return true;
}


//**********************************************************************************************************************99

bool ObjectQt3DPolyLine::pasteClipboars(QModelIndex index)
{

	QClipboard* clipboard = QGuiApplication::clipboard();
	QString text = clipboard->text().remove("\r");
	QStringList lst = text.split("\n");
	int point_start_index = _prop_vector.indexOf(_vertex_ptr);
	
	int row = index.row(),
		col = index.column(),
		count = lst.count(),
		point_count = _vertex_count->value().toInt(),
		shift = row - point_start_index;

	//  Попытка вставик в область общих свойств, что излишне
	if (shift < 0)
		return false;

	if (count + shift > point_count)
	{
		_vertex_count->setValue(QString::number(count + shift));
		//resizeVector();
	}

	for (int i = 0; i < count; i++)
	{
		index.sibling(row + i, col);
		//if (_model->flags(index) & Qt::ItemIsEditable)
		//	_model->setData(index, text);
	}

	return true;
}


QVector<PropertyAbstact*>& ObjectQt3DPolyLine::propVector()
{
	//return static_cast <const QVector<PropertyAbstact*>> (_prop_vector);
	return _prop_vector;
}

bool ObjectQt3DPolyLine::removeRows(int row, int count, const QModelIndex& index)
{
	for (int _row = 0; _row < count; _row++)
	    _prop_vector.removeAt(row);

	return false;
}

bool ObjectQt3DPolyLine::setNanoCadObject(AcDbEntity* pEnt)
{
	std::string str = CT2A(pEnt->isA()->name());


	// Проверяем тип примитива
	if (str != "AcDb3dPolyline")
		return false;
		
	
	AcDb3dPolyline* p_line = static_cast <AcDb3dPolyline*> (pEnt);

	NcGePoint3dArray gripPoints;
	NcDbIntArray osnapModes;
	NcDbIntArray geomIds;

	p_line->getGripPoints(gripPoints, osnapModes, geomIds);

	int count = gripPoints.size();
	_vertex_count->setValue(count);
	resizeVector();

	int point_start_index = _prop_vector.indexOf(_vertex_ptr);

	for (int j = 0; j < count; j++)
	{
		point_start_index;
		NcGePoint3d point = gripPoints.at(j);

		double x = point.x,
			y = point.y,
			z = point.z;

		_prop_vector[j + point_start_index]->setValue(QString("%1 %2 %3").arg(x).arg(y).arg(z));
		

		//ACHAR val_x[50], val_y[50], val_z[50];
		//acdbRToS(point.x, -1, 5, val_x);
		//acdbRToS(point.y, -1, 5, val_y);
		//acdbRToS(point.z, -1, 5, val_z);

		//acutPrintf(
		//	TEXT("\nТочка %d : (x = %s, y = %s, z = %s)"),
		//	j, val_x,
		//	val_y,
		//	val_z
		//);
	}

}
