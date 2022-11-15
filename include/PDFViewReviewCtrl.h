//
//  PDFViewReviewCtrl.h
//  FTRReview
//
//  Created by Michael Dobbs on 10/24/22.
//

#ifndef PDFViewReviewCtrl_hpp
#define PDFViewReviewCtrl_hpp

#include <wx/wx.h>
#include <wx/dataview.h>
#include "PDFView.h"


class PDFViewReviewCtrl: public wxDataViewListCtrl
{
public:
    PDFViewReviewCtrl() { }

    PDFViewReviewCtrl(wxWindow *parent,
                            wxWindowID id)
    {
        Create(parent, id);
    };

    bool Create(wxWindow *parent,
            wxWindowID id);

    void SetPDFView(wxPDFView* pdfView);

    void SetTextOutput( wxTextCtrl *ctrl) { textOutputCTRL = ctrl; };
    
    protected:

    private:

        void OnPDFDocumentReady(wxCommandEvent& event);

        void OnPDFDocumentClosed(wxCommandEvent& event);

        void OnSelectionChanged(wxDataViewEvent& event);

        void OnValueChanged(wxDataViewEvent& event);

        void OnSize(wxSizeEvent& event);
    
        wxString GetReviewTextResult(); //used to get review results for text box

        
        wxPDFView* m_pdfView;
    
        wxWeakRef<wxTextCtrl> textOutputCTRL;
    };
#endif /* PDFViewReviewCtrl_h */
