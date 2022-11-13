#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/frame.h>

const wxString version = "Alpha (2022-11-13)";

class FTRReviewApp : public wxApp
{
public:
	virtual bool OnInit();
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
