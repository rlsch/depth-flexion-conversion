add_custom_target(dependencies)
add_dependencies(dependencies opencv)
add_dependencies(dependencies eigen3)

if ((NOT ${opencv_built}) OR (NOT ${eigen3_built}))
    message(STATUS "Dependencies are not fullfilled!")
    set(DEPENDENCIES_BUILT FALSE)
else ()
    set(DEPENDENCIES_BUILT TRUE)
endif ()
