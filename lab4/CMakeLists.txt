project( lab4 )
cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package ( Qt4 REQUIRED QtGui QtCore)

include ( ${QT_USE_FILE} )

set ( lab4_HDRS
  lab4_widget.hxx
)

set ( lab2_SRCS
  lab4_widget.cxx
)

add_library ( lab4 SHARED ${lab2_SRCS} )
target_link_libraries ( lab4 ${QT_LIBRARIES} ${DESCRIBABLE_LIBRARY})

add_executable( run4 main.cxx )
target_link_libraries( run4 lab4 )
