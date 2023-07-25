#include "stdafx.h"

#include "hostUI.h"
#include "hostQt.h"
#include "PolyQtTable.h"


class NsdkDbReactor;
long gEntAcc = 0;            // Global entity count
NsdkDbReactor* gpDbr = NULL; // Pointer to database reactor
// Custom NcEditorReactor class for database
// event notification.
//  C++
// class NcEditorReactor : public NcRxEventReactor;

//  NcEditor virtual void addReactor(NcRxEventReactor* reactor);
void printDbEvent(const AcDbObject* pObj, const char* pEvent);

class NsdkDbReactor : public NcDbDatabaseReactor
{
public:
    virtual void objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj);
    virtual void objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj);
    virtual void objectErased(const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased);

};
// Called whenever an object is added to the database.
//
void NsdkDbReactor::objectAppended(const AcDbDatabase* db, const AcDbObject* pObj)
{
    printDbEvent(pObj, "objectAppended");
    acutPrintf (L" Db==%lx\n", (long)db);
    gEntAcc++;
    acutPrintf(L"Entity Count = %d\n", gEntAcc);
    acutPrintf(L"\nHello, NRX!\n");
}

// Called whenever an object in the database is modified.
//
void NsdkDbReactor::objectModified(const AcDbDatabase* db, const AcDbObject* pObj)
{
    printDbEvent(pObj, "objectModified");
    acutPrintf(L" Db==%lx\n", (long)db);
}

// Called whenever an object is erased from the database.
//
void NsdkDbReactor::objectErased(const AcDbDatabase* db,
    const AcDbObject* pObj, Adesk::Boolean pErased)
{
    if (pErased) {
        printDbEvent(pObj, "objectErased");
        gEntAcc--;
    }
    else {
        printDbEvent(pObj, "object(Un)erased");
        gEntAcc++;
    }
    acutPrintf(L" Db==%lx\n", (long)db);
    acutPrintf(L"Entity Count = %d\n", gEntAcc);
}

// Prints out the basic information about the object pointed
// to by pObj.
//
void printObj(const AcDbObject* pObj)
{
    if (pObj == NULL) {
        acutPrintf(L"(NULL)");
        return;
    }
    AcDbHandle objHand;
    char  handbuf[17];
    // Gets the handle as a string.
    //
    //pObj->getAcDbHandle(objHand);
    //objHand.getIntoAsciiBuffer(handbuf);
    acutPrintf(
        L"\n   (class==%s, handle==%s, id==%lx, db==%lx)",
        pObj->isA()->name(), handbuf,
        pObj->objectId().asOldId(), pObj->database());
}

// Prints the message passed in by pEvent; then 
// calls printObj() to print the information about
// the object that triggered the notification.
//

void printDbEvent(const AcDbObject* pObj, const char* pEvent)
{
    acutPrintf(L"  Event: NcEditorReactor::%s ", pEvent);
    printObj(pObj);
}





// Adds a reactor to the database to monitor changes.
// This can be called multiple times without any ill
// effect because subsequent calls are ignored.
//
void watchDb()
{
    if (gpDbr == NULL) {
        gpDbr = new NsdkDbReactor();
    }
    ncdbHostApplicationServices()->workingDatabase()->addReactor(gpDbr);


    acutPrintf(
        L"  Added Database Reactor to "
        "acdbHostApplicationServices()->workingDatabase().\n");
}

// Removes the database reactor.
//
void clearReactors()
{
    if (acdbHostApplicationServices()->workingDatabase() != NULL
        && gpDbr != NULL)
    {
        acdbHostApplicationServices()->workingDatabase()->removeReactor(gpDbr);
        delete gpDbr;
        gpDbr = NULL;
    }
}
