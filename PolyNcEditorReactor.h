#pragma once
#include "hostUI.h"
#include "hostQt.h"


#include <application.h>


class PolyNcEditorReactor :
    public NcEditorReactor
{
public:
//    NCRX_DECLARE_MEMBERS(PolyNcEditorReactor);

    //NCRX_CONS_DEFINE_MEMBERS(PolyNcEditorReactor, NcEditorReactor, 1);

    void pickfirstModified() override;

};

