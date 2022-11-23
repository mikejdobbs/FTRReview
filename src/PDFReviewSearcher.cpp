//
//  PDFReviewSearcher.cpp
//  FTRReview
//
//  Created by Michael Dobbs on 10/28/22.
//

#include <wx/regex.h>
#include <wx/textfile.h>

#include "PDFViewImpl.h"
#include "PDFReviewSearcher.h"
#include "PDFViewPages.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(wxPDFViewTextRangeList);


ReviewSearch::~ReviewSearch() {
    for (auto match: matches) {
        delete match; //TODO: FIX -causes crash
    }

    for (auto match: excludedMatches) {
        delete match;
    }

}

wxString Review::GetReviewTextResult() {
    //add page numbers
    wxString pages;
    for(auto reviewSearch : reviewSearches) {
        for(auto match : reviewSearch->matches) {
            pages << (match->GetPage()->GetIndex() + 1)  << ",";
        }
    }
    //page.replace
    
    return errorText; 
}

void Review::ignoreMatch(wxPDFViewTextRange *match){
    for (auto &reviewSearch : reviewSearches) {
        if (reviewSearch->matches.DeleteObject(match)) {
            reviewSearch->excludedMatches.push_back(match);
        }
    }
}
void Review::restoreMatch(wxPDFViewTextRange *match) {
    for (auto &reviewSearch : reviewSearches) {
        if (reviewSearch->excludedMatches.DeleteObject(match)) {
            reviewSearch->matches.push_back(match);
        } //end if
    } //end for
}


wxString Review::GetPagesForReviewSearch(ReviewSearch *reviewSearch) {
    wxString response;
    int lastPageIndex = 0;
    for (auto match: reviewSearch->matches ) {
        int pageIndex = match->GetPage()->GetIndex() + 1;
        
        //skip pages that have already been listed
        if (lastPageIndex == pageIndex) {
            continue;
        }
        
        if (response.length() > 0) {
            response += ", ";
        }
        
        response << pageIndex;
        lastPageIndex = pageIndex;
    }
    return response;
}

ReviewForInventionPatent::ReviewForInventionPatent() {
    reviewSearches.push_back(new ReviewSearch("invention",wxString("Possible Unreported Invention")));
    reviewSearches.push_back(new ReviewSearch("patent",wxString("Possible Unreported Patent")));
    reviewSearches.push_back(new ReviewSearch("USPTO",wxString("Possible Unreported Patent")));
}

//called before searching for matches
void ReviewForInventionPatent::PreReviewPage(wxString pageText) {
    
    //look for 15/123,456 or 15/123456 or \d{2,2}\/\d{3,3},*\d{3,3}
    wxRegEx patentAppRE("\\s\\d{2,2}\\/\\d{3,3},*\\d{3,3}\\s"); //        wxRegEx patentAppRE("\d{2,2}\/\d{3,3},*\d{3,3}");
    wxString processText = pageText;
    while ( patentAppRE.Matches(processText) ) {
        // Find the size of the first match and print it.
            size_t start, len;
            patentAppRE.GetMatch(&start, &len, 0);
            const wxString appNumberText = patentAppRE.GetMatch(processText,0);
            std::cout << "PatentApp: " << appNumberText;
            reviewSearches.push_back(new ReviewSearch(appNumberText,wxString("Possible Unreported Patent")));
            processText = processText.Mid (start + len); //keep movingt to make sure we got all the text
    }
    
    
    //7,123,456 numbers to flag
    wxRegEx patentRE("\\s\\d{1,2},*\\d{3,3},*\\d{3,3}\\s");
    processText = pageText;
    while ( patentRE.Matches(processText) ) {
        // Find the size of the first match and print it.
            size_t start, len;
            patentRE.GetMatch(&start, &len, 0);
            const wxString patentNumberText = patentRE.GetMatch(processText,0);
            std::cout << "Patent: " << patentNumberText;
            reviewSearches.push_back(new ReviewSearch(patentNumberText,wxString("Possible Unreported Patent")));
            processText = processText.Mid (start + len); //keep movingt to make sure we got all the text
    }
}

wxString ReviewForInventionPatent::GetReviewTextResult() {
    //Agregate all matched text together
    wxString response;
    
    for (auto &reviewSearch : reviewSearches) {
        if (!reviewSearch->matches.empty()) {
            response << "Please confirm all subject inventions have been reported to DOE via iEdison as the use of " << "\"" << reviewSearch->searchString << "\"" << " on page(s) " << GetPagesForReviewSearch(reviewSearch) << " indicate a potential subject invention." << wxTextFile::GetEOL();
        }
        
    }
    return response;
}


//Protective Markings

ReviewForProtectiveMarkings::ReviewForProtectiveMarkings() {
    reviewSearches.push_back(new ReviewSearch("proprietary",wxString("Proprietary")));
    reviewSearches.push_back(new ReviewSearch("export",wxString("Export Control")));
    reviewSearches.push_back(new ReviewSearch("limited rights",wxString("Limited Rights")));
    reviewSearches.push_back(new ReviewSearch("restricted rights",wxString("Restricted Rights")));
    reviewSearches.push_back(new ReviewSearch("classified",wxString("Classified")));
    reviewSearches.push_back(new ReviewSearch("secret",wxString("secret")));
    reviewSearches.push_back(new ReviewSearch("confidential",wxString("Confidential")));
    reviewSearches.push_back(new ReviewSearch("business sensitive",wxString("Business Sensitive")));
    reviewSearches.push_back(new ReviewSearch("trade secret",wxString("Trade Secret")));

}

//called before searching for matches -- Nothing to do yet for protective markings
void ReviewForProtectiveMarkings::PreReviewPage(wxString pageText) {
    
}

wxString ReviewForProtectiveMarkings::GetReviewTextResult() {
    //Agregate all matched text together
    wxString response;
    
    for (auto &reviewSearch : reviewSearches) {
        if (!reviewSearch->matches.empty()) {
            response << "Please remove the unauthorized use of " <<   "\"" << reviewSearch->searchString << "\"" << " on page(s) " << GetPagesForReviewSearch(reviewSearch)  << wxTextFile::GetEOL();
        }
        
    }
    return response;
}


//reviewforCopyright
ReviewForCopyright::ReviewForCopyright() {
    reviewSearches.push_back(new ReviewSearch("(c)",wxString("Copyright")));
    reviewSearches.push_back(new ReviewSearch("copyright",wxString("Copyright")));
}

wxString ReviewForCopyright::GetReviewTextResult() {
    wxString response;
    
    for (auto &reviewSearch : reviewSearches) {
        if (!reviewSearch->matches.empty()) {
            response << "Please remove " <<   "\"" << reviewSearch->searchString << "\"" << " on page(s) " << GetPagesForReviewSearch(reviewSearch)  <<  " or add the following Acknowledgement of the Government license." << wxTextFile::GetEOL();
            response << "Acknowledgement of Government Support and Government License\nThis work was generated with financial support from the U.S. Government through Contract/Award No. __________________, and as such the U.S. Government retains a paid-up, nonexclusive, irrevocable, world-wide license to reproduce, prepare derivative works, distribute copies to the public, and display publicly, by or on behalf of the Government, this work in whole or in part, or otherwise use the work for Federal purposes.";
        } //end if match
    }//end for
    return response;
}

//called before searching for matches
void ReviewForCopyright::PreReviewPage(wxString pageText) {
    //match to Copyright or (C), show error if no legend
    //match legend on page
    const wxString copyrightLegend = "U.S. Government retains a paid-up, nonexclusive, irrevocable, world-wide license to reproduce, prepare derivative works, distribute copies to the public, and display publicly, by or on behalf of the Government, this work in whole or in part, or otherwise use the work for Federal purposes.";

    if (pageText.Find(copyrightLegend) != wxNOT_FOUND) {
        reviewSearches.clear(); //no copyright issues if there is a legend
    }
    
    
}

