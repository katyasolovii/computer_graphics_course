

file(GLOB_RECURSE SRC
        include/*.h
        src/*.c
)

MESSAGE("GLAD: " ${SRC})


add_library(glad STATIC ${SRC})
target_include_directories(glad PUBLIC include)