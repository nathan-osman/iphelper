set(SRC
    main.cpp
)

add_executable(iphelper ${SRC})

set_target_properties(iphelper PROPERTIES
    CXX_STANDARD          14
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(iphelper Qt5::Network Qt5::Widgets)
