project( lab2 )
cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package ( Qt4 REQUIRED QtGui QtCore)

include ( ${QT_USE_FILE} )

set ( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" )
find_package ( Qwt6 REQUIRED)
include_directories( ${Qwt6_INCLUDE_DIR} )

set ( lab2_HDRS
  lab2_widget.hxx
)

set ( lab2_SRCS
  lab2_widget.cxx
)

add_library ( lab2 SHARED ${lab2_SRCS} )
target_link_libraries ( lab2 ${QT_LIBRARIES} ${Qwt6_LIBRARY}
  ${DESCRIBABLE_LIBRARY})
