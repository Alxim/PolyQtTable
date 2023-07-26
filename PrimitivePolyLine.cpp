#include "stdafx.h"
#include "PrimitivePolyLine.h"
#include "PropertyGroup.h"
#include "PropertyValueInt.h"
#include "PropertyValueDouble.h"
#include "PolyQtTableModel.h"
#include "PropertyValueCoordinates.h"
#include "PolyNcEditorReactor.h"

#include <QDebug>
#include <QClipboard>
#include <QGuiApplication>

PrimitivePolyLine::PrimitivePolyLine(QVector<PropertyAbstact*>* prop_vector)
	: _prop_vector(prop_vector), PrimitiveAbstract(prop_vector)
{
	_prop_vector->clear();

	PropertyGroup* pg_1 = new PropertyGroup("Общие");
	_prop_vector->append(pg_1);

	_vertex_count = new PropertyValueInt("Количество вершин", 4);
	_prop_vector->append(_vertex_count);

	PropertyValueInt* pvi_2 = new PropertyValueInt("Точность", 2);
	_prop_vector->append(pvi_2);

	PropertyGroup* pg_2 = new PropertyGroup("Вершины");
	_prop_vector->append(pg_2);

	// _point_start_index = 4, потому что 4 фиксированных поля


	resizeVector();

	connect(_vertex_count, SIGNAL(valueChanched()), this, SLOT(resizeVector()));
}

void PrimitivePolyLine::setModel(PolyQtTableModel* model)
{
	_model = model;
}

void PrimitivePolyLine::resizeVector()
{
	int curr_count = _prop_vector->count(),
		new_count = _vertex_count->value().toInt() + _point_start_index;


	if (curr_count == new_count)
		return;

	if (curr_count < new_count)
	{
		for (int i = curr_count; i < new_count; i++)
		{
			//double x = cos(3 * i) + i, y = sin(3 * i) + i, z = exp(double(i) / 10);
			double x = (pow(2, i)* (i%2) + i ) * 1000, 
				y = (pow(2, i) * ((i+1) % 2) + i) * 1000, 
				z = pow(2, i) * 1000;
			_prop_vector->append(new PropertyValueCoordinates(
				QString("Вершина %1").arg(i - _point_start_index + 1), 
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
			if (_model != nullptr)
				_model->removeRows(_model->rowCount() - 1, 1);

			curr_count--;
		}
	}

}


QString PrimitivePolyLine::className()
{
	return "PrimitivePolyLine";
}


bool PrimitivePolyLine::create()
{
#ifndef QT_PROJECT
	// polyline creation 
	NcGePoint3dArray ptArr;
	//for (int i = 0; i < 4; i++)
	//{
	//    ptArr[i].set((double)(i / 2), (double)(i % 2), 0.0);
	//}
	//NcDb2dPolyline* pNewPline = new NcDb2dPolyline(NcDb::k2dSimplePoly, ptArr, 0.0, NRX::kTrue);

	int count = _prop_vector->count();
	ptArr.setLogicalLength(count - _point_start_index + 1);

	for (int i = _point_start_index; i < count; i++)
	{
		double x = cos(3 * i) + i, y = sin (3 * i) + i, z = exp(double(i)/10);

		QString value = _prop_vector->at(i)->value().toString();
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

		ptArr[i - _point_start_index].set(x, y, z);
	}



	NcDb3dPolyline* pNewPline = new NcDb3dPolyline(NcDb::k3dSimplePoly, ptArr, NRX::kTrue);

	pNewPline->setColorIndex(3);


	//polygon mesh constructor without any parameter
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

#endif

	return true;
}

bool PrimitivePolyLine::pasteClipboars(QModelIndex index)
{
	QClipboard* clipboard = QGuiApplication::clipboard();
	QString text = clipboard->text().remove("\r");
	QStringList lst = text.split("\n");
	
	int row = index.row(),
		col = index.column(),
		count = lst.count(),
		point_count = _vertex_count->value().toInt(),
		shift = row - _point_start_index;

	//  Попытка вставик в область общих свойств, что излишне
	if (shift < 0)
		return false;

	if (count + shift > point_count)
	{
		_vertex_count->setValue(QString::number(count + shift));
		resizeVector();
	}

	for (int i = 0; i < count; i++)
	{
		index.sibling(row + i, col);
		if (_model->flags(index) & Qt::ItemIsEditable)
			_model->setData(index, text);
	}

	return true;
}
