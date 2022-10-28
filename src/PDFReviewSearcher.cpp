//
//  PDFReviewSearcher.cpp
//  FTRReview
//
//  Created by Michael Dobbs on 10/28/22.
//

#include "PDFReviewSearcher.h"

void PDFReviewSearcher::StopFind()
{
    m_selection.clear();
    m_findResults.clear();
    m_nextPageToSearch = -1;
    m_lastPageToSearch = -1;
    m_lastCharacterIndexToSearch = -1;
    m_currentFindIndex = -1;
    m_findText.clear();
}

long PDFReviewSearcher::Find(const wxString& text, int flags)
{
    if (m_pages.empty())
        return wxNOT_FOUND;

    bool firstSearch = false;
    int characterToStartSearchingFrom = 0;
    if (m_findText != text) // First time we search for this text.
    {
        firstSearch = true;
        wxVector<wxPDFViewTextRange> oldSelection = m_selection;
        StopFind();
        m_findText = text;

        if (m_findText.empty())
            return wxNOT_FOUND;

        if (oldSelection.empty()) {
            // Start searching from the beginning of the document.
            m_nextPageToSearch = -1;
            m_lastPageToSearch = GetPageCount() - 1;
            m_lastCharacterIndexToSearch = -1;
        } else {
            // There's a current selection, so start from it.
            m_nextPageToSearch = oldSelection[0].GetPage()->GetIndex();
            m_lastCharacterIndexToSearch = oldSelection[0].GetCharIndex();
            characterToStartSearchingFrom = oldSelection[0].GetCharIndex();
            m_lastPageToSearch = m_nextPageToSearch;
        }
    }

    if (m_findText.empty())
        return wxNOT_FOUND;

    bool caseSensitive = flags & wxPDFVIEW_FIND_MATCH_CASE;
    bool forward = (flags & wxPDFVIEW_FIND_BACKWARDS) == 0;

    // Move the find index
    if (forward)
        ++m_currentFindIndex;
    else
        --m_currentFindIndex;

    // Determine if we need more results
    bool needMoreResults = true;
    if (m_currentFindIndex == static_cast<int>(m_findResults.size()))
        m_nextPageToSearch++;
    else if (m_currentFindIndex < 0)
        m_nextPageToSearch--;
    else
        needMoreResults = false;

    while (needMoreResults
        && m_nextPageToSearch < GetPageCount()
        && m_nextPageToSearch >= 0)
    {
        int resultCount = FindOnPage(m_nextPageToSearch, caseSensitive, firstSearch, characterToStartSearchingFrom);
        if (resultCount)
            needMoreResults = false;
        else if (forward)
            ++m_nextPageToSearch;
        else
            --m_nextPageToSearch;
    }

    if (m_findResults.empty())
        return wxNOT_FOUND;

    // Wrap find index
    if (m_currentFindIndex < 0)
        m_currentFindIndex = m_findResults.size() - 1;
    else if (m_currentFindIndex >= (int) m_findResults.size())
        m_currentFindIndex = 0;

    // Select result
    m_selection.clear();
    wxPDFViewTextRange result = m_findResults[m_currentFindIndex];
    m_selection.push_back(result);
    int resultPageIndex = result.GetPage()->GetIndex();
    // Make selection visible


    return m_findResults.size();
}

int PDFReviewSearcher::FindOnPage(int pageIndex, bool caseSensitive, bool firstSearch, int WXUNUSED(characterToStartSearchingFrom))
{
    // Find all the matches in the current page.
    unsigned long flags = caseSensitive ? FPDF_MATCHCASE : 0;
    wxMBConvUTF16LE conv;
    FPDF_SCHHANDLE find = FPDFText_FindStart(
        m_pages[pageIndex].GetTextPage(),
#ifdef __WXMSW__
        reinterpret_cast<FPDF_WIDESTRING>(m_findText.wc_str(conv)),
#else
        reinterpret_cast<FPDF_WIDESTRING>((const char*)m_findText.mb_str(conv)),
#endif
        flags, 0);

    wxPDFViewPage& page = m_pages[pageIndex];

    int resultCount = 0;
    while (FPDFText_FindNext(find))
    {
        wxPDFViewTextRange result(&page,
            FPDFText_GetSchResultIndex(find),
            FPDFText_GetSchCount(find));

        if (!firstSearch &&
            m_lastCharacterIndexToSearch != -1 &&
            result.GetPage()->GetIndex() == m_lastPageToSearch &&
            result.GetCharIndex() >= m_lastCharacterIndexToSearch)
        {
          break;
        }

        //AddFindResult(result);
        ++resultCount;
    }

    FPDFText_FindClose(find);

    return resultCount;
}
