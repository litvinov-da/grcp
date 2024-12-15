project(client)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

set(TARGET_SOURCES main.cpp clientwidget.h clientwidget.cpp clientwidget.ui
    model/broadcasttablemodel.h model/broadcasttablemodel.cpp
    model/broadcasttableitem.h model/broadcasttableitem.cpp)

add_executable(${PROJECT_NAME} ${TARGET_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network

    broadcastUtil
    api
)
