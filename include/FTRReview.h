#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/app.h>
#include <wx/frame.h>
#include "wx/dnd.h"

const wxString VERSION = "1.0 (2023-3-24)";

class FTRReviewApp : public wxApp
{
public:
	virtual bool OnInit();
};

class DnDFile : public wxFileDropTarget
{
public:
    DnDFile(wxFrame *pOwner = nullptr) { m_pOwner = pOwner; }

    virtual bool OnDropFiles(wxCoord x, wxCoord y,
                             const wxArrayString& filenames) override;

private:
    wxFrame *m_pOwner;
};

class FTRReviewFrame : public wxFrame
{
    public:
        FTRReviewFrame();
        void OnButton(wxCommandEvent &evt);

    private:
        wxMenuBar *menubar;
        wxMenu *menuFileBar;
    
        void onAbout(wxCommandEvent& WXUNUSED(event));
        void onQuit(wxCommandEvent& WXUNUSED(event));


};

wxIMPLEMENT_APP(FTRReviewApp);

#endif
