set(_SPHINX_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR})

include(FindPackageHandleStandardArgs)

# We are likely to find Sphinx near the Python interpreter
find_package(Python REQUIRED)
if(Python_FOUND)
    get_filename_component(_PYTHON_DIR "${Python_EXECUTABLE}" DIRECTORY)
    set(
        _PYTHON_PATHS
        "${_PYTHON_DIR}"
        "${_PYTHON_DIR}/bin"
        "${_PYTHON_DIR}/Scripts")
endif()

find_program(
    SPHINX_EXECUTABLE
    NAMES sphinx-build sphinx-build.exe
    HINTS ${_PYTHON_PATHS}
    REQUIRE)
mark_as_advanced(SPHINX_EXECUTABLE)

find_package_handle_standard_args(Sphinx DEFAULT_MSG SPHINX_EXECUTABLE)