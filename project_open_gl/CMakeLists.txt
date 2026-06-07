cmake_minimum_required(VERSION 3.20)

project(project_gl)
set(CMAKE_CXX_STANDARD 17)

# Підключає папку renderer як підпроєкт зі своїм CMakeLists.txt —
# збирає рушій і всі його залежності (glfw, glad, glm, assimp)
add_subdirectory(renderer)

# Назва виконуваного файлу
set(APP_TARGET app)
# Абсолютний шлях до папки додатку
set(APP_DIR ${CMAKE_SOURCE_DIR}/app)

# Збирає всі .cpp і .h з app/src/ у змінну APP_SRC
file(GLOB_RECURSE APP_SRC
        "${APP_DIR}/src/*.cpp"
        "${APP_DIR}/src/*.h"
)

# Створює виконуваний файл з усіх зібраних вихідників
add_executable(${APP_TARGET} ${APP_SRC})

# Вказує де шукати .h файли рушія —
# PRIVATE означає лише для цього таргету, не для тих хто його використовує
target_include_directories(${APP_TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/renderer/renderer  # заголовки рушія (Camera.h, SceneManager.h...)
        ${CMAKE_SOURCE_DIR}/renderer/utils     # утиліти рушія
        ${APP_DIR}/src                         # робочий C++ код
)

# Лінкує бібліотеки до виконуваного файлу
target_link_libraries(${APP_TARGET} PRIVATE
        renderer  # рушій - основна бібліотека
        assimp    # завантаження 3D моделей (.obj)
        glfw      # створення вікна і обробка вводу
        glad      # завантаження функцій OpenGL
        glm       # математика: вектори, матриці, кватерніони
)

# Після збірки копіює app/res/ (шейдери, моделі) поруч з .exe (виконуваний файл, тобто програма яку можна запустити) —
# щоб відносні шляхи до файлів працювали при запуску
add_custom_command(
        TARGET ${APP_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/app/res
        $<TARGET_FILE_DIR:${APP_TARGET}>/res
)