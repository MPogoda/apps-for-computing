project( lab1 )
cmake_minimum_required( VERSION 2.8.6 )
set ( CMAKE_AUTOMOC true )

add_definitions( -std=c++11 )

find_package ( Qt4 REQUIRED QtCore QtGui)

include ( ${QT_USE_FILE} )

set ( lab1_HDRS
  lab1_widget.hxx
)

set ( lab1_SRCS
  main.cxx
  lab1_widget.cxx
)

add_executable( lab1 ${lab1_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries( lab1 ${QT_LIBRARIES} )
