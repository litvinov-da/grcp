project(server)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

set(TARGET_SOURCES "main.cpp" "server.h" "server.cpp"
    "serverwidget.h" "serverwidget.cpp" "serverwidget.ui" "pingservice.h" "pingservice.cpp"
    "pingserver.h" "pingserver.cpp")

add_executable(${PROJECT_NAME} ${TARGET_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt6::Core
	Qt6::Network
	Qt6::Widgets

	spdlog::spdlog

	broadcastUtil
	api
)
