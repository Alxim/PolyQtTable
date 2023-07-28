#include "stdafx.h"

#include "hostUI.h"
#include "hostQt.h"
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

	// ��� ������
	ads_name sset; 

	ads_name ename; // ��� ��� �������� ������
	// AcDbObjectId entId;  // ������������� �������

	// ������� ��� ���������� ������� � ���� ������ �����
	acedSSGet(TEXT("_I"), NULL, NULL, NULL, sset);
	long length;
	acedSSLength(sset, (Adesk::Int32*)&length);

	if (sset == NULL)
	{
		acutPrintf(TEXT("\nsset is NULL"));
		return;
	}
	
	acutPrintf( TEXT( "\n\n���������� ���������� ��������:% d" ), length );

	for (long i = 0; i < length; i++)
	{
		acutPrintf(TEXT("\n\n������� ������ :% d"), i);

		acedSSName(sset, i, ename);
		AcDbObjectId objId;
		
		if (ename == NULL)
		{
			acutPrintf(TEXT("\nename is NULL"));
			continue;
		}

		// ���������� ID ��� �������� �� ads_name � AcDbObjectId
		int result = acdbGetObjectId(objId, ename);
		if (result != Acad::eOk)
		{
			acutPrintf(TEXT("\n������ ��������� �������������� ������� %d"), result);
			continue;
		}
		
		if (objId == NULL)
		{
			acutPrintf(TEXT("\nobjId is NULL"));
			continue;
		}

		// �������� ��������� �� ������� �������
		AcDbEntity* pEnt;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);

		// �������� ��������� � �������� �������, ���������� ���� ���� ��������
		if (es == Acad::eWasOpenForWrite)
		{
			acutPrintf(TEXT("\n������ ��������� ������� %s"), es);
			continue;
		}

		printObj(pEnt);
		std::string str = CT2A( pEnt->isA()->name() ); 

		// ��������� ��� ���������.
		//  TODO ����� ������ ��������� ��������� � ������ ��� ����� ����� ��������� ����� �������

		if (str == "AcDb3dPolyline") 
		{
			acutPrintf(TEXT("\n ������� 3� ��������� !"));
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
					TEXT("\n����� %d : (x = %s, y = %s, z = %s)"),
					j, val_x,
					val_y,
					val_z
				);
			}
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

