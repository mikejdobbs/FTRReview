#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/mike/Documents/xcode/FTRReview/xcode
  make -f /Users/mike/Documents/xcode/FTRReview/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/mike/Documents/xcode/FTRReview/xcode
  make -f /Users/mike/Documents/xcode/FTRReview/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/mike/Documents/xcode/FTRReview/xcode
  make -f /Users/mike/Documents/xcode/FTRReview/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/mike/Documents/xcode/FTRReview/xcode
  make -f /Users/mike/Documents/xcode/FTRReview/xcode/CMakeScripts/ReRunCMake.make
fi

