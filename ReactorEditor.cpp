#include "stdafx.h"

#include "hostUI.h"
#include "hostQt.h"
#include "PolyQtTable.h"
#include "nc2ac.h"
#include "objptr.h"

void printObj(const AcDbObject* pObj);

class PolyNcEditorReactor :
	public NcEditorReactor
{
public:
	void pickfirstModified() override;
};

void PolyNcEditorReactor::pickfirstModified()
{
	acutPrintf(
		L"\nvoid PolyNcEditorReactor::pickfirstModified().\n");

	// Имя набора
	ads_name sset; 

	ads_name ename; // Имя для элемента набора
	// AcDbObjectId entId;  // Идентификатор объекта

	// Выбрать все выделенные объекты в базе данных графа
	acedSSGet(TEXT("_I"), NULL, NULL, NULL, sset);
	long length;
	acedSSLength(sset, (Adesk::Int32*)&length);

	if(sset == NULL)
		acutPrintf(TEXT("\nsset is NULL"));
	
	acutPrintf( TEXT( "\n\nКоличество выделенных объектов:% d" ), length );

	for (long i = 0; i < length; i++)
	{
		acutPrintf(TEXT("\n\nNumber of names:% d"), i);

		acedSSName(sset, i, ename);
		AcDbObjectId objId;
		
		acutPrintf(TEXT("\nNames of Entities: %s"), ename);
		if(ename == NULL)
			acutPrintf(TEXT("\nename is NULL") );

		// Вычисление ID для перехода от ads_name к AcDbObjectId
		int result = acdbGetObjectId(objId, ename);
		if (result != Acad::eOk)
		{
			acutPrintf(TEXT("\nОшибка получение идентификатора объекта %d"), result);
			continue;
		}
		

		acutPrintf(TEXT("\nNames of Entities: %s"), objId);
		if (objId == NULL)
		{
			acutPrintf(TEXT("\nobjId is NULL"));
			continue;
		}

		// Получить указатель на текущий элемент
		AcDbEntity* pEnt;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
		// Выберите полилинию в качестве границы, пропустите этот цикл напрямую
		if (es == Acad::eWasOpenForWrite)
		{
			acutPrintf(TEXT("\nОшибка получение объекта %s"), es);
			continue;
		}

		printObj(pEnt);
		

		std::string str = CT2A( pEnt->isA()->name() ); 

		if (str == "AcDb3dPolyline") 
		{

			acutPrintf(TEXT("\nУспех!!!"));


			//AcDbBlockReference* pInsert;
			Acad::ErrorStatus es;

			// Проверяем тип примитива.
			acdbGetObjectId(objId, ename);
			if (Acad::eOk != (es = acdbOpenAcDbEntity(pEnt,
				objId,
				AcDb::kForRead)))
			{
				acutPrintf(L"\nНельзя получить доступ к примитиву.\n");
				return;
			}

			acutPrintf(
				L"  acdbGetObjectId\n");

			//pInsert = AcDbBlockReference::cast(pEnt);
			//if (!pInsert)
			//{
			//	acutPrintf(L"\nВыбрали не вставку блока.\n");
			//	pEnt->close();
			//	return;
			//}

			// Получаем objectID определения блока.
			AcDbObjectId blockDefId = pEnt->blockId();
				//pInsert->blockTableRecord();

			// Закрываем вставку блока.
			// pInsert->close();

			// Открываем определение блока.
			//AcDbBlockTableRecord* pBlkRecord;
			//if (Acad::eOk != (es = acdbOpenObject(pBlkRecord,
			//	blockDefId,
			//	AcDb::kForRead)))
			//{
			//	acutPrintf(L"\nНельзя получить доступ к определению блока.\n");
			//	return;
			//}

			//// Получаем имя определения блока.
			//const TCHAR* pBlkName;
			//es = pBlkRecord->getName(pBlkName);

			//AcDbBlockTableRecordIterator* pIterator = NULL;
			//pBlkRecord->newIterator(pIterator);

			//for (pIterator->start(); !pIterator->done(); pIterator->step())
			//{
			//	AcDbObjectId idEnt;
			//	pIterator->getEntityId(idEnt);
			//	AcDbObjectPointer<AcDbEntity> pEnt1(idEnt, AcDb::kForRead);
			//	if (pEnt1.openStatus() == Acad::eOk)
			//	{
			//		// Получаем свойства примитива в блоке (слой, цвет, типлинии и т.д.)
			//		pEnt1->
			//	}
			//}
			//delete pIterator;

			//pBlkRecord->close();
			//if ((Acad::eOk != es) || !pBlkName)
			//{
			//	acutPrintf(L"\nНе можем получить имя блока.\n");
			//	return;
			//}

			//acutPrintf(L"\nИмя блока: '%s'\n", pBlkName);


			//return;


			AcDb3dPolyline* p_line = static_cast <AcDb3dPolyline*> (pEnt);

			NcGePoint3dArray gripPoints;
			NcDbIntArray osnapModes;
			NcDbIntArray geomIds;

			p_line->getGripPoints(gripPoints, osnapModes, geomIds);


			for (int j = 0; j < gripPoints.size(); j++)
			{
				NcGePoint3d point = gripPoints.at(j);
				
				ACHAR val_x[50], val_y[50], val_z[50];
				acdbRToS(point.x, -1, 5, val_x);
				acdbRToS(point.y, -1, 5, val_y);
				acdbRToS(point.z, -1, 5, val_z);

				acutPrintf(
					TEXT("\nТочка %d : (x = %s, y = %s, z = %s)"),
					j, val_x,
					val_y,
					val_z
				);

			}

			double test = 3.5;
			double number = 5.66;

			acutPrintf(TEXT("\n%.6q0"), number);
			acutPrintf(TEXT("\n%.10q*"), test);

			QString str = QString::number(test) + "    " + QString::number(number);

			acutPrintf(TEXT("\n%s"), str.toStdString());

			ACHAR valStr[50];
			//-1 использует текущие единицы базы данных чертежа
			int unit = -1;
			// точность – 5 знаков после запятой
			int prec = 5;
			acdbRToS(15.20024, unit, prec, valStr);

			acutPrintf(_T("плавающее: %s\n"), valStr);
			// Для научного используем 1
			unit = 1;
			acdbRToS(15.20024, unit, prec, valStr);
			acutPrintf(_T("Научное : %s\n"), valStr);

			// Для десятичного представления используем 2
			unit = 2;
			acdbRToS(15.20024, unit, prec, valStr);
			acutPrintf(_T("Десятичное  : %s\n"), valStr);

			// Для инженерного используем 3
			unit = 3;
			acdbRToS(15.20024, unit, prec, valStr);
			acutPrintf(_T("Инженерное : %s\n"), valStr);

			// Для архитектурного используем 4
			unit = 4;
			acdbRToS(15.20024, unit, prec, valStr);
			acutPrintf(_T("Архитектурное : %s\n"), valStr);

			// Для дробного используем 5
			unit = 5;
			acdbRToS(15.20024, unit, prec, valStr);
			acutPrintf(_T("Дробное : %s\n"), valStr);

		}
		else
			acutPrintf(TEXT("%s"), str);


		acutPrintf(TEXT("\nTest 10"));

	}

	acutPrintf(TEXT("\nEnd for..."));

	// Выполнять другие операции

	//AcDbObject obj;
	//acedSSGet("dd", )
	//(sset, (Adesk::Int32*)&length);


	acedSSFree(sset);
	return;

	//// Позволим пользователю выбрать вставку блока.
	//ads_name ename;
	//ads_point pt;

	//acedEntSel(L"\nВыберите вставку блока: ",
	//	ename,
	//	pt);

	//AcDbObjectId objId;
	//AcDbEntity* pEnt;
	//AcDbBlockReference* pInsert;
	//Acad::ErrorStatus es;

	//// Проверяем тип примитива.
	//acdbGetObjectId(objId, ename);
	//if (Acad::eOk != (es = acdbOpenAcDbEntity(pEnt,
	//	objId,
	//	AcDb::kForRead)))
	//{
	//	acutPrintf(L"\nНельзя получить доступ к примитиву.\n");
	//	return;
	//}

	//acutPrintf(
	//	L"  acdbGetObjectId\n");

	//pInsert = AcDbBlockReference::cast(pEnt);
	//if (!pInsert)
	//{
	//	acutPrintf(L"\nВыбрали не вставку блока.\n");
	//	pEnt->close();
	//	return;
	//}

	//// Получаем objectID определения блока.
	//AcDbObjectId blockDefId = pInsert->blockTableRecord();

	//// Закрываем вставку блока.
	//pInsert->close();

	//// Открываем определение блока.
	//AcDbBlockTableRecord* pBlkRecord;
	//if (Acad::eOk != (es = acdbOpenObject(pBlkRecord,
	//	blockDefId,
	//	AcDb::kForRead)))
	//{
	//	acutPrintf(L"\nНельзя получить доступ к определению блока.\n");
	//	return;
	//}

	//// Получаем имя определения блока.
	//const TCHAR* pBlkName;
	//es = pBlkRecord->getName(pBlkName);
	//pBlkRecord->close();
	//if ((Acad::eOk != es) || !pBlkName)
	//{
	//	acutPrintf(L"\nНе можем получить имя блока.\n");
	//	return;
	//}

	//acutPrintf(L"\nИмя блока: '%s'\n", pBlkName);


	//return;


	return;
	
}

PolyNcEditorReactor* N_EDITOR = nullptr;

void createNcEditorReactor()
{
	if (N_EDITOR == nullptr)
		N_EDITOR = new PolyNcEditorReactor();

	NcEditor::instance()->addReactor(N_EDITOR);
}


void clearNcEditorReactor()
{
	if (N_EDITOR != nullptr)
	{
		NcEditor::instance()->removeReactor(N_EDITOR);
		delete N_EDITOR;
		N_EDITOR = nullptr;
	}
}

