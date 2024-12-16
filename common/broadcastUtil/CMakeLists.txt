project(broadcastUtil)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(TARGET_SOURCES broadcastlistener.h broadcastlistener.cpp
    broadcastsender.h broadcastsender.cpp
    private/broadcastcommon.h
    private/broadcastmessage.h private/broadcastmessage.cpp
    error.h error.cpp)

add_library(${PROJECT_NAME} STATIC ${TARGET_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Network

    networkUtil
)
