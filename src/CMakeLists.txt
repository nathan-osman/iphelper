set(SRC
    main.cpp
    mainwindow.h
    mainwindow.cpp
    resource.qrc
)

add_executable(iphelper ${SRC})

set_target_properties(iphelper PROPERTIES
    CXX_STANDARD          11
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(iphelper PRIVATE)
target_link_libraries(iphelper Qt5::Network Qt5::Widgets)

install(TARGETS iphelper RUNTIME DESTINATION bin)
