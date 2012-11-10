project( lab2 )
cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

add_definitions(-std=c++11)

find_package ( Qt4 REQUIRED QtGui QtCore)

include ( ${QT_USE_FILE} )

set ( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
find_package ( Qwt6 REQUIRED)
include_directories( ${Qwt6_INCLUDE_DIR} )

set ( lab2_HDRS
  lab2_widget.hxx
)

set ( lab2_SRCS
  main.cxx
  lab2_widget.cxx
)

add_executable ( lab2 ${lab2_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( lab2  ${QT_LIBRARIES} ${Qwt6_LIBRARY} )
