#include "stdafx.h"

#include "hostUI.h"
#include "hostQt.h"
#include "nc2ac.h"
#include "objptr.h"
#include "ObjectQtAbstract.h"
#include <QHash>

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

	if (sset == NULL)
	{
		acutPrintf(TEXT("\nsset is NULL"));
		return;
	}
	
	acutPrintf( TEXT( "\n\nКоличество выделенных объектов:% d" ), length );

	for (long i = 0; i < length; i++)
	{
		acutPrintf(TEXT("\n\nТекущий объект :% d"), i);

		acedSSName(sset, i, ename);
		AcDbObjectId objId;
		
		if (ename == NULL)
		{
			acutPrintf(TEXT("\nename is NULL"));
			continue;
		}

		// Вычисление ID для перехода от ads_name к AcDbObjectId
		int result = acdbGetObjectId(objId, ename);
		if (result != Acad::eOk)
		{
			acutPrintf(TEXT("\nОшибка получение идентификатора объекта %d"), result);
			continue;
		}
		
		if (objId == NULL)
		{
			acutPrintf(TEXT("\nobjId is NULL"));
			continue;
		}

		// Получить указатель на текущий элемент
		NcDbEntity* pEnt;
		Acad::ErrorStatus es = ncdbOpenNcDbEntity(pEnt, objId, AcDb::kForRead);

		// Выберите полилинию в качестве границы, пропустите этот цикл напрямую
		if (es == Acad::eWasOpenForWrite)
		{
			acutPrintf(TEXT("\nОшибка получение объекта %s"), es);
			continue;
		}

		printObj(pEnt);
		std::string str = CT2A( pEnt->isA()->name() ); 

		ObjectQtAbstract* temp = ObjectQtAbstract::OBJECT_QT_HASH.value(QString::fromStdString(str), nullptr);

		acutPrintf(TEXT("\n OBJECT_QT_HASH.size() = %d "), ObjectQtAbstract::OBJECT_QT_HASH.size());
		acutPrintf(TEXT("\n %s - str "), str);
		acutPrintf(TEXT("\n %s - OBJECT_QT_HASH.keys "), ObjectQtAbstract::OBJECT_QT_HASH.keys().join(", ").toStdString());

		if (temp != nullptr)
		{
			acutPrintf(TEXT("\n temp != nullptr"));
			if (temp->setNanoCadObject(pEnt))
				return;
		}
		else
		{
			acutPrintf(TEXT("\n temp == nullptr \n %d "), ObjectQtAbstract::OBJECT_QT_HASH.size() );
		}

		delete pEnt;
	}

	acutPrintf(TEXT("\nEnd for..."));
	acedSSFree(sset);
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

