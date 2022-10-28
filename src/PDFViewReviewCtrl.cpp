//
//  PDFViewReviewCtrl.cpp
//  FTRReview
//
//  Created by Michael Dobbs on 10/24/22.
//

#include "PDFViewReviewCtrl.h"

bool PDFViewReviewCtrl::Create(wxWindow *parent, wxWindowID id) {
    //listctrl = new wxDataViewListCtrl( parent, wxID_ANY );
    
    if ( !wxDataViewListCtrl::Create( parent, id ) )
        return false;
    
    m_pdfView = NULL;
    
    AppendToggleColumn( "Toggle", wxDATAVIEW_CELL_ACTIVATABLE,10,  wxALIGN_LEFT,wxDATAVIEW_COL_RESIZABLE );
    AppendTextColumn( "Page", wxDATAVIEW_CELL_ACTIVATABLE,10,  wxALIGN_LEFT,wxDATAVIEW_COL_RESIZABLE );
    AppendTextColumn( "Issue" );
    
    wxVector<wxVariant> data;
    data.push_back( wxVariant(true) );
    data.push_back( wxVariant("1") );
    data.push_back( wxVariant("Proprietary Marking") );
    AppendItem( data );
    
    data.clear();
    data.push_back( wxVariant(false) );
    data.push_back( wxVariant("3") );
    data.push_back( wxVariant("Invention Report") );
    AppendItem( data );
    
    Bind(wxEVT_DATAVIEW_SELECTION_CHANGED, &PDFViewReviewCtrl::OnSelectionChanged, this);
    Bind(wxEVT_SIZE, &PDFViewReviewCtrl::OnSize, this);

}


void PDFViewReviewCtrl::SetPDFView(wxPDFView* pdfView) {
    m_pdfView = pdfView;
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
    
    m_pdfView->GoToPage(pageNumber);
    event.Skip();
}

void PDFViewReviewCtrl::OnPDFDocumentClosed(wxCommandEvent& event)
{
#ifdef __WXOSX__
    AssociateModel(NULL);
#else
    // Workaround for potential bug in generic dataview impl
    wxObjectDataPtr<wxPDFViewBookmarksModel> treeModel(new wxPDFViewBookmarksModel(NULL));
    AssociateModel(treeModel.get());
#endif

    event.Skip();
}

void PDFViewReviewCtrl::OnPDFDocumentReady(wxCommandEvent& event)
{
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

