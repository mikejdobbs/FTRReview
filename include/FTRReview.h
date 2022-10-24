#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#include <wx/app.h>
#include <wx/frame.h>

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
};

wxIMPLEMENT_APP(FTRReviewApp);

#endif
