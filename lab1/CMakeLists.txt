project( lab1 )
cmake_minimum_required( VERSION 2.8.6 )
set ( CMAKE_AUTOMOC true )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package ( Qt4 REQUIRED QtCore QtGui)

include ( ${QT_USE_FILE} )

set ( lab1_HDRS
  lab1_widget.hxx
)

set ( lab1_SRCS
  lab1_widget.cxx
)

add_library( lab1 SHARED ${lab1_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries( lab1 ${QT_LIBRARIES} )
