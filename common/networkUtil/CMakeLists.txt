project(networkUtil)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(TARGET_SOURCES networknodeinfo.h networknodeinfo.cpp)

add_library(${PROJECT_NAME} STATIC ${TARGET_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Network
)
