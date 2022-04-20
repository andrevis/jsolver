
# creates static library target from all files in specified directory
function(add_static_library_from_dir NAME DIR)
    # build list of source files
    set(LIB_SOURCE_PATH "${PROJECT_SOURCE_DIR}/${DIR}")
    file(GLOB_RECURSE  SOURCE_FILES
        "${LIB_SOURCE_PATH}/*.h"
        "${LIB_SOURCE_PATH}/*.cc"
        "${LIB_SOURCE_PATH}/*.cpp"
        "${LIB_SOURCE_PATH}/*.hpp"
    )

    # define library
    add_library(${NAME} STATIC ${SOURCE_FILES})

    # include own directory (for dependent libraries)
    target_include_directories(${NAME} PUBLIC ${LIB_SOURCE_PATH})
endfunction()

# creates executable target from all files in specified directory
function(add_executable_from_dir NAME DIR)
    # build list of source files
    set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/${DIR}")
    file(GLOB_RECURSE  SOURCE_FILES
        "${SOURCE_PATH}/*.h"
        "${SOURCE_PATH}/*.cc"
        "${SOURCE_PATH}/*.cpp"
        "${SOURCE_PATH}/*.hpp"
    )

    # create target
    add_executable(${NAME} ${SOURCE_FILES})

    # include own directory (for dependent libraries)
    target_include_directories(${NAME} PUBLIC ${SOURCE_PATH})
endfunction()

# creates object target from all files in specified directory
function(add_target_object NAME DIR)

    # build list of source files
    set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/${DIR}")
    file(GLOB_RECURSE  SOURCE_FILES
        "${SOURCE_PATH}/*.h"
        "${SOURCE_PATH}/*.cc"
        "${SOURCE_PATH}/*.cpp"
        "${SOURCE_PATH}/*.hpp"
    )

    # create target
    add_library(${NAME} OBJECT ${SOURCE_FILES})

    # include own directory (for dependent libraries)
    target_include_directories(${NAME} PUBLIC ${SOURCE_PATH})

    # include directories
    foreach(dir IN LISTS ARGN)
        target_include_directories(${NAME} PUBLIC ${dir})
    endforeach()

endfunction()

function(add_executable_from_objs NAME OBJ)

    # define executable and it's sources from obj-targets
    add_executable(${NAME} $<TARGET_OBJECTS:${OBJ}>)

    foreach(obj IN LISTS ARGN)
        target_sources(${NAME} PUBLIC $<TARGET_OBJECTS:${obj}>)
    endforeach()

endfunction()

# creates shared library target
function(add_shared_library_from_objs NAME OBJ)

    # create target
    add_library(${NAME} SHARED  $<TARGET_OBJECTS:${OBJ}>)

    foreach(obj IN LISTS ARGN)
        target_sources(${NAME} PRIVATE $<TARGET_OBJECTS:${obj}>)
    endforeach()

endfunction()

function(execute cmd arg path result)
    execute_process(COMMAND ${cmd} ${arg}
                    WORKING_DIRECTORY ${path}
                    RESULT_VARIABLE res
                    OUTPUT_VARIABLE out
    )
    set(${result} ${res} PARENT_SCOPE)
endfunction(execute)