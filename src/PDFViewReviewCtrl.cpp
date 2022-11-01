//
//  PDFViewReviewCtrl.cpp
//  FTRReview
//
//  Created by Michael Dobbs on 10/24/22.
//

#include "PDFViewReviewCtrl.h"
#include "PDFViewPages.h"
#include "PDFViewImpl.h"

bool PDFViewReviewCtrl::Create(wxWindow *parent, wxWindowID id) {
    if ( !wxDataViewListCtrl::Create( parent, id ) )
        return false;
    
    m_pdfView = NULL;
    
    AppendToggleColumn( "Toggle", wxDATAVIEW_CELL_ACTIVATABLE,10,  wxALIGN_LEFT,wxDATAVIEW_COL_RESIZABLE );
    AppendTextColumn( "Page", wxDATAVIEW_CELL_ACTIVATABLE,25,  wxALIGN_LEFT,wxDATAVIEW_COL_RESIZABLE );
    AppendTextColumn( "Issue" );
    
    Bind(wxEVT_DATAVIEW_SELECTION_CHANGED, &PDFViewReviewCtrl::OnSelectionChanged, this);
    Bind(wxEVT_SIZE, &PDFViewReviewCtrl::OnSize, this);

}


void PDFViewReviewCtrl::SetPDFView(wxPDFView* pdfView) {
    if (m_pdfView != NULL)
    {
        // Disconnect events
        m_pdfView->Unbind(wxEVT_PDFVIEW_DOCUMENT_READY, &PDFViewReviewCtrl::OnPDFDocumentReady, this);
        m_pdfView->Unbind(wxEVT_PDFVIEW_DOCUMENT_CLOSED, &PDFViewReviewCtrl::OnPDFDocumentClosed, this);
    }

    m_pdfView = pdfView;

    if (m_pdfView != NULL)
    {
        // Connect events
        m_pdfView->Bind(wxEVT_PDFVIEW_DOCUMENT_READY, &PDFViewReviewCtrl::OnPDFDocumentReady, this);
        m_pdfView->Bind(wxEVT_PDFVIEW_DOCUMENT_CLOSED, &PDFViewReviewCtrl::OnPDFDocumentClosed, this);
    }
    
}

//called when user selects 
void PDFViewReviewCtrl::OnSelectionChanged(wxDataViewEvent& event)
{
    const int rowID = GetSelectedRow();
    if (rowID == wxNOT_FOUND) {
        event.Skip();
        return;
    }
    
    wxString rowText = GetTextValue(rowID, 1);
    int pageNumber;
    rowText.ToInt(&pageNumber);
    
    m_pdfView->GoToPage(pageNumber - 1); //pages are 0-indexed
    
    event.Skip();
}

void PDFViewReviewCtrl::OnPDFDocumentClosed(wxCommandEvent& event)
{
    return;
    event.Skip();
}

void PDFViewReviewCtrl::OnPDFDocumentReady(wxCommandEvent& event)
{
    //Review TODO:Multithreaded?
    m_pdfView->ReviewPDF();
    
    m_pdfView->GetImpl()->clearSelections();
    
    

    //TODO: Cleanup
    std::vector<ReviewResult> results = m_pdfView->GetReviewResultSortedByPage();
    for (std::vector<ReviewResult>::iterator result = results.begin(); result != results.end(); ++result) {
        
            wxString pageNumber;
            pageNumber << result->page;

        
            wxVector<wxVariant> data;
            data.push_back( wxVariant(true) );
            data.push_back( wxVariant(pageNumber) );
            data.push_back( wxVariant(result->review.description) );
            AppendItem( data );
        
        m_pdfView->GetImpl()->SetSelection(result->match);

    } //end search for review

    event.Skip();
}

void PDFViewReviewCtrl::OnSize(wxSizeEvent& event)
{
#if defined(wxUSE_GENERICDATAVIEWCTRL)
    // automatically resize our only column to take the entire control width
    if ( GetColumnCount() )
    {
        wxSize size = GetClientSize();
        GetColumn(0)->SetWidth(size.x);
    }
#endif
    event.Skip();
}

