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

void updateTable();


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
	acutPrintf(TEXT("\nstart resizeVector()"));

	int point_start_index = _prop_vector.indexOf(_vertex_ptr);

	int curr_count = _prop_vector.count(),
		new_count = _vertex_count->value().toInt() + point_start_index;

	//acutPrintf(TEXT("\ncurr_count = %d "
	//	"\nnew_count = %d"
	//"\n_vertex_count->value().toInt() = %d"
	//	"\npoint_start_index = %d"), curr_count, new_count, _vertex_count->value().toInt(), point_start_index);

	if (curr_count == new_count)
		return;

	PolyQtTableModel* _model = q_ptr->getModel();

	if (curr_count < new_count)
	{
		acutPrintf(TEXT("\nAdd new row in table"));

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
		acutPrintf(TEXT("\nRemove row from table"));
		while (curr_count != new_count)
		{
			_model->removeRows(_model->rowCount() - 1, 1);

			curr_count--;
		}
	}

}



bool ObjectQt3DPolyLine::create()
{
	// Создание 3Д полелинии
	NcGePoint3dArray ptArr;

	int point_start_index = _prop_vector.indexOf(_vertex_ptr);
	int count = _prop_vector.count();
	ptArr.setLogicalLength(count - point_start_index);

	QString value;
	//  Заполнение NcGePoint3dArray из данных интерфейса
	for (int i = point_start_index; i < count; i++)
	{	
		value = _prop_vector.at(i)->value().toString(); 

		Point3D point;
		if ( ! splitStringToPoint3d(value, point))
			return false;

		ptArr[i - point_start_index].set(point.x, point.y, point.z);
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

bool ObjectQt3DPolyLine::mayPasteClipboars(const QModelIndex& index)
{
	if (index.column() == 0)
		return false;

	int point_start_index = _prop_vector.indexOf(_vertex_ptr);
	if (index.row() < point_start_index)
		return false;

	return true;
}

//************************************************************************************************************************

bool ObjectQt3DPolyLine::pasteClipboars(QTableView* table)
{
	QModelIndex index = table->currentIndex();
	QClipboard* clipboard = QGuiApplication::clipboard();
	QString text = clipboard->text().remove("\r");
	QStringList lst = text.split("\n");
	int point_start_index = _prop_vector.indexOf(_vertex_ptr);
	
	int row = index.row(),
		col = index.column(),
		count = lst.count(),
		point_count = _vertex_count->value().toInt(),
		shift = row - point_start_index;


//	ACHAR t1[50], t2[50], t3[50],
//		t4[50], t5[50], t6[50];
//
//	acdbRToS(point_start_index, -1, 5, t1);
//	acdbRToS(row, -1, 5, t2);
//	acdbRToS(col, -1, 5, t3);
//	acdbRToS(count, -1, 5, t4);
//	acdbRToS(point_count, -1, 5, t5);
//	acdbRToS(shift, -1, 5, t6);
//
//	acutPrintf(
//		TEXT(R"(
//------------------------------------
//
//point_start_index = %s
//row = %s
//col = %s
//count = %s
//point_count = %s
//shift = %s)"),
//		t1, t2, t3, t4, t5, t6
//	);

	//  Попытка вставик в область общих свойств, что излишне
	if (shift < 0)
		return false;

	if (count == 1)
		return _prop_vector.at(row)->setValue(text);
	

	//  Вставка нескольких строк
	acutPrintf(TEXT("\nstart for"));
	int i = 0;
	for (; i < count; i++)
	{
		acutPrintf(TEXT("\nfor i = %d"), i);
		if (shift + i >= point_count)
		{
			acutPrintf(TEXT("\nInsert new point %d"), shift + i);
			
			Point3D point;
			if (!splitStringToPoint3d(lst[i], point))
			{
				acutPrintf(TEXT("\nError sintax in line: %s"), lst[i]);
				return false;
			}

			acutPrintf(TEXT("\nsplit end"));


			PolyQtTableModel* model = q_ptr->getModel();
			QModelIndex index_temp = model->index(_prop_vector.indexOf(_vertex_count), 1);
			
			point_count++;
			acutPrintf(TEXT("\nset value point_count: %d"), point_count);
			model->setData(index_temp, point_count);
			qApp->processEvents();

			if (row + i >= _prop_vector.count() )
			{
				acutPrintf(TEXT("\nResize Error"));
				return false;
			}
		}

		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
		AcAxDocLock lockDoc(pDb);

		acutPrintf(TEXT("\nInsert old point %d"), shift + i);
		if (!_prop_vector.at(row + i)->setValue(lst[i]))
			return false;

		QModelIndex index_select = index.sibling(row + i, 1);
		table->setCurrentIndex(index_select);
	}

	updateTable();	
	return true;
}


QVector<PropertyAbstact*>& ObjectQt3DPolyLine::propVector()
{
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
	PolyQtTableModel* model = q_ptr->getModel();
	QString data;

	for (int j = 0; j < count; j++)
	{
		point_start_index;
		NcGePoint3d point = gripPoints.at(j);

		data = QString("%1 %2 %3").arg(point.x).arg(point.y).arg(point.z);

		acutPrintf(TEXT("\nSet model data %s"), data.toStdString());
		
		_prop_vector[j + point_start_index]->setValue(data);

		//ACHAR val_x[50], val_y[50], val_z[50];
		//acdbRToS(point.x, -1, 5, val_x);
		//acdbRToS(point.y, -1, 5, val_y);
		//acdbRToS(point.z, -1, 5, val_z);

		//acutPrintf(
		//	TEXT("\nPoint %d : (x = %s, y = %s, z = %s)"),
		//	j, val_x,
		//	val_y,
		//	val_z
		//);
	}

	updateTable();
}


bool ObjectQt3DPolyLine::update(AcDbEntity* pEnt)
{
	acutPrintf(TEXT("Start update"));

	if (pEnt == nullptr)
	{
		acutPrintf(TEXT("pEnt == nullptr"));
		return false;
	}

	if (pEnt == NULL)
	{
		acutPrintf(TEXT("pEnt == NULL"));
		return false;
	}

	std::string str = CT2A(pEnt->isA()->name());


	// Проверяем тип примитива
	if (str != "AcDb3dPolyline")
		return false;


	AcDb3dPolyline* p_line = static_cast <AcDb3dPolyline*> (pEnt);

	NcGePoint3dArray gripPoints;
	NcDbIntArray osnapModes;
	NcDbIntArray geomIds;

	NcDbGripDataPtrArray arra;
	NcGeVector3d vec;
	vec.x = 4;

	p_line->getGripPoints(gripPoints, osnapModes, geomIds);
	//  p_line->getGripPoints(arra, 5, 5, vec, true);

	//  NcDbObjectIterator* iter = p_line->vertexIterator();


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
	//  NcDb::kForRead - Замыкать фигуру, если не ставить, то будет разорванная


	NcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, NcDb::kForWrite);
	pBlockTable->close();


	NcDbObjectId plineObjId;
	pBlockTableRecord->appendAcDbEntity(plineObjId, p_line);


	NcDbObjectIterator* pVertIter = p_line->vertexIterator();
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

		//location.set(location.x + 1000, location.y + 1000, location.z + 1000);

		pVertex->setPosition(location);

		pVertex->close();
		//NcDbPolygonMeshVertex* polyVertex = new NcDbPolygonMeshVertex(pVertex->position());
		//pMesh->appendVertex(polyVertex);
		//polyVertex->close();
	}
	delete pVertIter;


	pBlockTableRecord->appendAcDbEntity(pMesh);
	pBlockTableRecord->close();
	p_line->close();
	pMesh->close();

	// p_line->setAttributes(NcGiDrawableTraits)
	
	int count = gripPoints.size();

	//_vertex_count->setValue(count);
	//resizeVector();

	//int point_start_index = _prop_vector.indexOf(_vertex_ptr);
	//PolyQtTableModel* model = q_ptr->getModel();
	//QString data;

	//for (int j = 0; j < count; j++)
	//{
	//	point_start_index;
	//	NcGePoint3d point = gripPoints.at(j);
	//	point.x = point.x + 1000;
	//	point.y = point.y + 1000;
	//	point.z = point.z + 1000;

	//	gripPoints.insertAt(j, point);
	//}

	//NcDb3dPolylineVertex* vet = new NcDb3dPolylineVertex();


	// p_line->appendVertex(vet);  //appendVertex()
	//(gripPoints);

	//AcDbObjectPointer<AcDbXrecord> pLyrFltr(p_line->objectId(), AcDb::kForWrite);

	//if (Acad::eOk == pLyrFltr.openStatus())
	//{
	//	pLyrFltr->erase();
	//}

	//delNanoObject(p_line->objectId());
	//p_line->erase();
	//p_line->close();
	//delete p_line;
	//p_line->upgradeOpen();

	//NcDbBlockTable* pBlockTable;
	//ncdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, NcDb::kForRead);
	////  NcDb::kForRead - Замыкать фигуру, если не ставить, то будет разорванная


	//NcDbBlockTableRecord* pBlockTableRecord;
	//pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, NcDb::kForWrite);
	//pBlockTable->close();

	//NcDbObjectId plineObjId;
	//pBlockTableRecord->upgradeOpen();

	//pBlockTableRecord->close();
	//p_line->close();

	acutPrintf(TEXT("End update"));

	updateTable();
}


void ObjectQt3DPolyLine::delNanoObject(NcDbObjectId objId)
{
	//Get the layer table of the current graphic
	AcDbLayerTable* pLayerTbl;
	acdbHostApplicationServices()
		->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);

	//pLayerTbl->has(

	//Determine whether it contains the layer table record of the specified name
	if (!pLayerTbl->has(objId)) {
		pLayerTbl->close();
		return;
	}
	//Get the pointer of the specified layer table record
	AcDbLayerTableRecord* pLayerTblRcd;
	NCHAR* name;
	pLayerTbl->getAt(name, objId, AcDb::kForWrite);
	//(objId, pLayerTblRcd, AcDb::kForWrite);
	//Set a "delete" mark for it

	

	pLayerTblRcd->erase();
	pLayerTblRcd->close();		//Close the layer table
	pLayerTbl->close();
}

bool ObjectQt3DPolyLine::splitStringToPoint3d(QString str, Point3D& point)
{
	bool ok;
	QStringList lst = str.split(" ");

	switch (lst.count())
	{
	case 3:
		point.z = lst.at(2).toDouble(&ok);
		if (!ok)
			return false;
	case 2:
		point.x = lst.at(0).toDouble(&ok);
		if (!ok)
			return false;

		point.y = lst.at(1).toDouble(&ok);
		if (!ok)
			return false;
		break;
	default:
		break;
	}
	return true;
}