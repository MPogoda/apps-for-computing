project( afc )

cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package( Qt4 REQUIRED QtGui QtCore )

include( ${QT_USE_FILE} )

set( afc_SRCS
  main.cxx
  main_widget.cxx
)

set( describable_SRCS
  describable.hxx
)

add_library( describable ${describable_SRCS} ${QT_LIBRARIES} )

add_subdirectory( lab1 )

add_executable( afc lab1 ${afc_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )

add_dependencies( lab1 describable)

add_dependencies( afc lab1)

target_link_libraries( afc lab1 describable ${QT_LIBRARIES} )
