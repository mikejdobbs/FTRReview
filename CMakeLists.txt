cmake_minimum_required(VERSION 3.24)

# Name the project
project(FTRReview)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Requirments
if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES ../sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
    list(APPEND SRC_FILES ../../src/osx/carbon/wxmac.icns)
endif()


#WXwidgets

find_package(wxWidgets REQUIRED COMPONENTS net core adv base)
include(${wxWidgets_USE_FILE})

#PDFIUM
set(WITH_PDFIUM_SOURCE true)
set(PDFium_DIR ${PROJECT_SOURCE_DIR}/PDFium)
set (CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/PDFium)
find_package(PDFium)


#sources
set(SOURCES 

	src/FTRReview.cpp
	src/PDFView.cpp
	src/PDFViewImpl.cpp
	src/PDFViewActivityPanel.cpp
	src/PDFViewArtProvider.cpp
	src/PDFViewBookmarks.cpp
	src/PDFViewBookmarksCtrl.cpp
	src/PDFViewDocumentFrame.cpp
	src/PDFViewDocumentPanel.cpp
	src/PDFViewPages.cpp
	src/PDFViewPrintout.cpp
	src/PDFViewTextRange.cpp
	src/PDFViewThumbnailListBox.cpp
	src/PDFViewThumbnailListBoxImpl.cpp

	include/FTRReview.h
	include/PDFView.h
	include/PDFViewArtProvider.h
	include/PDFViewBookmarksCtrl.h
	include/PDFViewDocumentFrame.h
	include/PDFViewDocumentPanel.h
	include/PDFViewThumbnailListBox.h
	include/PDFViewActivityPanel.h
	include/PDFViewBookmarks.h
	include/PDFViewImpl.h
	include/PDFViewPages.h
	include/PDFViewPrintout.h
	include/PDFViewTextRange.h
	include/PDFViewThumbnailListBoxImpl.h
)

#target
add_executable(FTRReview ${SOURCES})

target_link_libraries(FTRReview ${wxWidgets_LIBRARIES})
target_link_libraries(FTRReview pdfium)

#includes
target_include_directories(FTRReview PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(FTRReview PRIVATE ${PROJECT_SOURCE_DIR}/art)



# To compile this program all you need to do is ask cmake.  You would type
# these commands from within the directory containing this CMakeLists.txt
# file:
#   mkdir build
#   cd build
#   cmake ..
#   cmake --build . --config Release
#
# The cmake .. command looks in the parent folder for a file named
# CMakeLists.txt, reads it, and sets up everything needed to build program.
# Also, note that CMake can generate Visual Studio or XCode project files.  So
# if instead you had written:
#   cd build
#   cmake .. -G Xcode
#
# You would be able to open the resulting Xcode project and compile and edit
# the example programs within the Xcode IDE.  CMake can generate a lot of
# different types of IDE projects.  Run the cmake -h command to see a list of
# arguments to -G to see what kinds of projects cmake can generate for you.  It
# probably includes your favorite IDE in the list.




