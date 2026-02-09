cmake_minimum_required(VERSION 3.27)

if (NOT EXISTS ${PYBIND11_LIB})
    message(FATAL_ERROR "Pybind 11 library is not valid! ")
endif ()
list(APPEND CMAKE_PREFIX_PATH ${PYBIND11_LIB})
find_package(pybind11 CONFIG REQUIRED)

set(PYENGINE PyEngine)

pybind11_add_module(${PYENGINE}
        src/Module.cpp
        src/Module.h
        src/Core.cpp
        src/Core.h
        src/Basic.cpp
        src/Basic.h
)

target_link_libraries(${PYENGINE} PRIVATE
        ${PROJECT_NAME}
        ${TARGET_LIBS}
        pybind11::pybind11
        pybind11::module
)