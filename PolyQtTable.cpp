//
// Копирайт (С) 2019, ООО «Нанософт разработка». Все права защищены.
// 
// Данное программное обеспечение, все исключительные права на него, его
// документация и сопроводительные материалы принадлежат ООО «Нанософт разработка».
// Данное программное обеспечение может использоваться при разработке и входить
// в состав разработанных программных продуктов при соблюдении условий
// использования, оговоренных в «Лицензионном договоре присоединения
// на использование программы для ЭВМ «Платформа nanoCAD»».
// 
// Данное программное обеспечение защищено в соответствии с законодательством
// Российской Федерации об интеллектуальной собственности и международными
// правовыми актами.
// 
// Используя данное программное обеспечение,  его документацию и
// сопроводительные материалы вы соглашаетесь с условиями использования,
// указанными выше. 
//

#include "stdafx.h"
#include "PolyQtTable.h"
#include "PrimitivePolyLine.h"

#include <qdebug.h>
#include <QMessageBox>
#include <QKeyEvent>
#include <QClipboard>

PolyQtTableWidget::PolyQtTableWidget(QWidget *parent)
  : QWidget(parent)
{
  ui.setupUi(this);

  QObject::connect(ui.create, SIGNAL(clicked()), this, SLOT(showDialog()));


  PrimitivePolyLine* prim = new PrimitivePolyLine(&prop_vector);
  primitives.append(prim);

  model = new PolyQtTableModel(&prop_vector);
  //poly_line->setModel(model);

  ui.tableView->setModel(model);

  ui.tableView->resizeColumnsToContents();
  ui.tableView->hideColumn(2);
  ui.tableView->horizontalHeader()->hide();
  ui.tableView->verticalHeader()->hide();

  ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui.tableView->setItemDelegate(new PolyQtTableDelegat());

}

PolyQtTableWidget::~PolyQtTableWidget()
{}

void PolyQtTableWidget::showDialog()
{
    qDebug() << "Test print";
#ifndef QT_PROJECT
    // polyline creation 
    NcGePoint3dArray ptArr;
    ptArr.setLogicalLength(4);
    for (int i = 0; i < 4; i++)
    {
        ptArr[i].set((double)(i / 2), (double)(i % 2), 0.0);
    }
    //NcDb2dPolyline* pNewPline = new NcDb2dPolyline(NcDb::k2dSimplePoly, ptArr, 0.0, NRX::kTrue);
    

    NcDb3dPolyline* pNewPline = new NcDb3dPolyline (NcDb::k3dSimplePoly, ptArr, NRX::kTrue);
    
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
    /**/
    QString str = "Test";
    QMessageBox::about(this, "QMessageBox", 
        QString("Hello, this is %1 signals and slots!").arg(str)
    );
}

void PolyQtTableWidget::on_create_clicked()
{
    //primitives.at(ui.comboBox->currentIndex())->resizeVector();
}

void PolyQtTableWidget::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {
        case Qt::Key_V:
        if(pe->modifiers() == Qt::ControlModifier)
        {
            clipboarsPaste();
        }
        break;

        case Qt::Key_Z:
/*			if((pe->modifiers() & Qt::ControlModifier) &&
                ui->Pravka->isChecked())
              // ui->Spis->isActiveWindow())
            {

                ui->Spis->blockSignals(true);
                SpisItem(ui->Spis->currentRow());
                //Mess(3, "Test", QString("Вы нажали Ctrl + Z"));
                Otmena(ui->Spis->currentRow());
                SpisCombo(ui->Spis->currentRow(), ui->Spis->currentColumn());
                ui->Spis->blockSignals(false);
            }
*/
        break;

        case Qt::Key_Delete:			//Делит
//			poisk_per::Del();
        break;


        default:
        ;
    }

    pe->accept();
}

void PolyQtTableWidget::clipboarsPaste()
{
    QModelIndex index = ui.tableView->currentIndex();


    if( ! model->flags(index) & Qt::ItemIsEditable)
        return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    QString text = clipboard->text();

    model->setData(index, text);

    ui.tableView->setCurrentIndex(index);

}
