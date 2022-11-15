//
//  PDFReviewSearcher.hpp
//  FTRReview
//
//  Created by Michael Dobbs on 10/28/22.
//

#ifndef PDFReviewSearcher_h
#define PDFReviewSearcher_h

#include <wx/wx.h>
#include <wx/regex.h>

#include "PDFViewTextRange.h"

WX_DECLARE_LIST(wxPDFViewTextRange, wxPDFViewTextRangeList);

//used to hold search results
class ReviewSearch {
    public:
        wxString searchString = wxString("proprietary");
        wxString description = wxString("Description"); //showed by gui for each match
        wxPDFViewTextRangeList matches;
        wxPDFViewTextRangeList excludedMatches; //holds matches which have been excluded

        ReviewSearch(wxString searchString,wxString description) : searchString(searchString),description(description) {}
};

//A Class to hold information for each review
class Review {
    public:
        int id = 0;
        wxVector<ReviewSearch> reviewSearches;
        
        wxString description = wxString("Proprietary Data Marking");
        wxString errorText = wxString("Pages $(PAGES) include proprietary data markings, which are not acceptable under the terms of your agreement,");
    
        virtual wxString GetReviewTextResult(); //returns text for box
        virtual void PreReviewPage(wxString pageText) {wxLogError("Error, Undefined PreReviewPage");}
        wxString GetPagesForReviewSearch(const ReviewSearch &reviewSearch);

        void ignoreMatch(wxPDFViewTextRange *match);
        void restoreMatch(wxPDFViewTextRange *match);


};


class ReviewForInventionPatent : public Review {
    public:
         ReviewForInventionPatent();
         wxString GetReviewTextResult(); //returns text for box
         void PreReviewPage(wxString pageText);
        
};

class ReviewForProtectiveMarkings : public Review  {
    public:
        ReviewForProtectiveMarkings();
         wxString GetReviewTextResult(); //returns text for box
         void PreReviewPage(wxString pageText);

};

//ReviewForCopyright

class ReviewForCopyright : public Review  {
    public:
         wxString GetReviewTextResult(); //returns text for box
         void PreReviewPage(wxString pageText);
        ReviewForCopyright();
    
    private:
};

/* Class used to display results orderd by page */
class ReviewResult {
    public:
        int page = 1; //1-indexed //used on display
        wxString description;
    
        Review *review;  //Used to report toggles back to DB
        wxPDFViewTextRange *match; //Used to add/subtract based on toggle
        ReviewResult (int page, wxString description, Review *review, wxPDFViewTextRange *match) : page(page),description(description),review(review),match(match) {};
};

#endif /* PDFReviewSearcher_h */


