project( lab5 )
cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package ( Qt4 REQUIRED QtGui QtCore)

include ( ${QT_USE_FILE} )

set ( lab5_SRCS
  lab5_widget.cxx
)

add_library ( lab5 SHARED ${lab5_SRCS} )

target_link_libraries ( lab5 ${QT_LIBRARIES} ${DESCRIBABLE_LIBRARY})
