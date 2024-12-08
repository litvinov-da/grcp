project(server)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

set(TARGET_SOURCES "main.cpp" "server.h" "server.cpp"  "serverwidget.h" "serverwidget.cpp" "serverwidget.ui")

add_executable(${PROJECT_NAME} ${TARGET_SOURCES})

include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.15.0
)

FetchContent_MakeAvailable(spdlog)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt6::Core
	Qt6::Network
	Qt6::Widgets

	spdlog::spdlog
)