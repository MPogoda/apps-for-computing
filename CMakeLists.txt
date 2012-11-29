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

set( SRCS
  main.cxx
  main_widget.cxx
)

set( DESCRIBABLE_LIBRARY describable )

add_subdirectory( describable )
add_subdirectory( lab1 )
add_subdirectory( lab2 )

add_executable( afc ${SRCS} )

add_dependencies( lab1 describable )
add_dependencies( lab2 describable )

add_dependencies( afc
                  lab1 lab2
)

target_link_libraries( afc lab1 lab2 ${QT_LIBRARIES} )
