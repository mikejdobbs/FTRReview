#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sharedptr.h>
#include <wx/webview.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/aboutdlg.h>

#include "FTRReview.h"

#include "PDFViewDocumentFrame.h"

//Nothing is really in this file, except the option to load a document and create wxPDFViewDocumentFrame

bool FTRReviewApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler());
	
	FTRReviewFrame *frame = new FTRReviewFrame();
	frame->Show(true);

	return true;
}

FTRReviewFrame::FTRReviewFrame() : wxFrame(NULL, wxID_ANY, "FTRReview")
{
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	wxButton* btn = new wxButton(this, wxID_ANY, "Load document");

	topsizer->Add(btn, 0, wxCENTER);

	this->SetSizer(topsizer);
	this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FTRReviewFrame::OnButton, this);
    
    //menu
    menubar = new wxMenuBar;
    menuFileBar  = new wxMenu;
    
    menuFileBar->Append(wxID_ABOUT, wxT("&About"));
    menuFileBar->Append(wxID_EXIT, wxT("Q&uit"));
    
    menubar->Append(menuFileBar, wxT("&File"));
    SetMenuBar(menubar);
    
    wxString strFile("Drop files here!"), strText("Drop text on me");

    wxListBox *m_ctrlFile  = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 1, &strFile,
                                    wxLB_HSCROLL | wxLB_ALWAYS_SB );
    
    topsizer->Add(m_ctrlFile, 1, wxEXPAND);

    
    m_ctrlFile->DragAcceptFiles(true);
    m_ctrlFile->SetDropTarget(new DnDFile());
    
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FTRReviewFrame::onQuit));
    Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FTRReviewFrame::onAbout));
}

void FTRReviewFrame::OnButton(wxCommandEvent &evt)
{
	wxFileDialog dlg(this, "Select Document", wxEmptyString, wxEmptyString, "*.pdf", wxCENTRE|wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxPDFViewDocumentFrame* frame = new wxPDFViewDocumentFrame(this);

		if (frame->LoadFile(dlg.GetPath()))
			frame->Show();
	}
}




void FTRReviewFrame::onAbout(wxCommandEvent& WXUNUSED(event)) {
//    wxMessageDialog *dial = new wxMessageDialog(NULL,
//       "Final Technical Report Review\n\n Michael Dobbs\nIntellectual Propertly Law(IPL)\nU.S. Department of Energy\nMike.Dobbs@science.doe.gov\n\n" + version, wxT("About"), wxOK | wxICON_INFORMATION);
//    dial->ShowModal();
    
    wxAboutDialogInfo aboutInfo;
       aboutInfo.SetName("FTRReview");
       aboutInfo.SetVersion(MY_APP_VERSION_STRING);
       aboutInfo.SetDescription("Final Technical Report Review\n\nSpecial Thanks to Glen Drysdale, Tim Harney, Tim Harney, Paula Leon, and Cindy Ridge");
       aboutInfo.SetCopyright("(C) 2022 License:wxWindows licence\nwww.wxwidgets.org/about/licence/");
       aboutInfo.AddDeveloper("Michael J. Dobbs");
    
       wxAboutBox(aboutInfo);
}

void FTRReviewFrame::onQuit(wxCommandEvent& WXUNUSED(event)) {
    Close();
    
}

bool DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
    size_t nFiles = filenames.GetCount();
    wxString str;
    str.Printf( "%d files dropped", (int)nFiles);

    for (auto filename: filenames) {
        wxPDFViewDocumentFrame* frame = new wxPDFViewDocumentFrame(m_pOwner);
        if (frame->LoadFile(filename))
            frame->Show();
    }
    

    return true;
}
