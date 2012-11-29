project( describable )

cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package( Qt4 REQUIRED QtGui QtCore )

include( ${QT_USE_FILE} )

set( describable_SRCS
  describable.cxx
)

add_library( describable SHARED ${describable_SRCS} )

target_link_libraries( describable ${QT_LIBRARIES} )
