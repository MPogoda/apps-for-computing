project( lab6 )
cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package ( Qt4 REQUIRED QtGui QtCore)

include ( ${QT_USE_FILE} )

set ( lab6_SRCS
  lab6_widget.cxx
)

add_library ( lab6 SHARED ${lab6_SRCS} )

target_link_libraries ( lab6 ${QT_LIBRARIES} ${DESCRIBABLE_LIBRARY})

add_executable( run6 main.cxx )
target_link_libraries( run6 lab6 )
