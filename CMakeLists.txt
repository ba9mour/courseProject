cmake_minimum_required(VERSION 3.14)
project(TaskManagerJSON LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Подтягиваем nlohmann/json
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# 2. Находим Qt6 Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 3. Пути к вашим заголовочным файлам ( папка headers/ )
include_directories(${PROJECT_SOURCE_DIR}/headers)

# 4. Список исходников и заголовков
set(SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
)

set(HEADERS
    headers/Task.h
    headers/TaskManager.h
    src/gui/MainWindow.h
)

add_executable(task_manager
    ${SOURCES}
    ${HEADERS}
)

# 5. Линкуем с nlohmann_json и Qt6::Widgets
target_link_libraries(task_manager
    PRIVATE nlohmann_json::nlohmann_json
            Qt6::Widgets
)

# 6. Включаем автоматическую генерацию MOC
set_target_properties(task_manager PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)