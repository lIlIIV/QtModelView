cmake_minimum_required(VERSION 3.1.0)

# ... Boost

find_package(Boost COMPONENTS system REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# ... Qt

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)


set(SRCS
  main.cpp
  main-window.cpp
  my-delegate.cpp
  my-model.cpp
  my-style.cpp
  my-view.cpp
  tools/dispatch.cpp
  widgets/clickable.cpp
  widgets/document-viewer.cpp
  widgets/thumbnail-editor.cpp
)

set(HDRS
  main-window.hh
  my-delegate.hh
  my-model.hh
  my-style.hh
  my-view.hh
  tools/dispatch.hh
  widgets/clickable.hh
  widgets/document-viewer.hh
  widgets/thumbnail-editor.hh
) 

#set(UIS
#  a.ui 
#)

set(QRCS
  resource.qrc
)

# ...

# QT5_WRAP_UI(UI_HDRS ${UIS})
QT5_ADD_RESOURCES(RESOURCES_RCC ${QRCS})

# ...

# add_library(ipso-secretaire-static STATIC ${helloworld_SOURCES} ${helloworld_FORMS_HEADERS})
# qt5_use_modules(ipso-secretaire-static Widgets Network)

# ...

# if(APPLE)
#    # Required for breakpad
#    SET(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -w")
# endif()

# ...

add_executable(${project_name} WIN32 ${SRCS} ${HRDS} ${RESOURCES_RCC})

#add_executable(${project_name} WIN32 ${SRC} ${HRDS} ${UI_HDRS} ${RESOURCES_RCC})
target_link_libraries(${project_name} boost_system boost_date_time Qt5::Core Qt5::Widgets)

# add_executable(ipso-secretaire WIN32 MACOSX_BUNDLE main.cpp ${helloworld_RESOURCES_RCC} ${OSX_ICON_FILES})
# target_link_libraries(ipso-secretaire ipso-secretaire-static ipso-webapi ipso-crash-report-handler boost_system boost_date_time pthread cups poppler poppler-qt5 poppler-cpp)

# ...
