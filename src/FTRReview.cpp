#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sharedptr.h>
#include <wx/webview.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

#include "FTRReview.h"

#include "PDFViewDocumentFrame.h"

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
