cmake_minimum_required(VERSION 3.16)
project(TourAgency)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Sql)   # ← ДОБАВИЛ Sql

set(CORE_SOURCES
    core/TourAgency.cpp
)

set(CORE_HEADERS
    core/BaseEntity.h
    core/Date.h
    core/Client.h
    core/Tour.h
    core/Order.h
    core/AdditionalService.h
    core/TourAgency.h
)

set(UI_SOURCES
    main.cpp
    mainwindow.cpp
)

set(UI_HEADERS
    mainwindow.h
)

set(UI_FORMS
    mainwindow.ui
)

add_executable(TourAgency
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${UI_SOURCES}
    ${UI_HEADERS}
    ${UI_FORMS}
)

target_link_libraries(TourAgency
    Qt6::Widgets
    Qt6::Core
    Qt6::Sql          # ← ДОБАВИЛ
)

if (WIN32)
    set_target_properties(TourAgency PROPERTIES WIN32_EXECUTABLE TRUE)
endif()