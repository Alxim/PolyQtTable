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
	acutPrintf(TEXT("\nNumber of Entities: %d"), length);

	acutPrintf(TEXT("\nNames of select: %s"), sset);

	if(sset == NULL)
		acutPrintf(TEXT("\nsset is NULL"));
	else
		acutPrintf(TEXT("\nsset is NOT NULL"));
	
	for (long i = 0; i < length; i++)
	{
		acutPrintf(TEXT("\n\nNumber of names:% d"), i);

		acedSSName(sset, i, ename);
		
		AcDbObjectId objId;
		
		acutPrintf(TEXT("\nNames of Entities: %s"), ename);
		if(ename == NULL)
			acutPrintf(TEXT("\nename is NULL") );
		else
			acutPrintf(TEXT("\nename is NOT NULL"));


		// Вычисление ID для перехода от ads_name к AcDbObjectId
		int result = acdbGetObjectId(objId, ename);
		if (result != Acad::eOk)
		{
			acutPrintf(TEXT("\nОшибка получение идентификатора объекта %d"), result);
			continue;
		}
		

		acutPrintf(TEXT("\nNames of Entities: %s"), objId);
		if (objId == NULL)
			acutPrintf(TEXT("\nobjId is NULL"));
		else
			acutPrintf(TEXT("\nobjId is NOT NULL"));

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
		}
		else
			acutPrintf(TEXT( "%s"), str);


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


	//NcDbObjectIdArray arr;
	//int err = ncedGetCurrentSelectionSet(arr);

	//acutPrintf(L"  err::%d ", err);

	//if (err != Nano::eOk)
	//	return;

	//if (arr == NULL)
	//{
	//	acutPrintf(L"  NULL ");
	//	return;
	//}

	//int _size = arr.size();

	//acutPrintf(L"  err::%d ", _size );
	//return;

	//nds_name sset;
	//err = acedSSGet(_T("_I"), NULL, NULL, NULL, sset);
	//if (err != RTNORM)
	//	return;

	//acutPrintf(L"  err::%d ", err);
	//acutPrintf(L"  sset::%d ", sset[0]);

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

