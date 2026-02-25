/***************************************************************
 * Name:      GraApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2024-12-19
 * Copyright:  ()
 * License:
 **************************************************************/

#include "GraApp.h"

//(*AppHeaders
#include "GraMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(GraApp);

bool GraApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	GraDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)



    return wxsOK;

}
