cmake_minimum_required (VERSION 3.1)
add_subdirectory (src)
if (R_INCLUDE_DIR)
    add_subdirectory (R)
else()
    message(WARNING "Skipping RMorphyex library, R development libraries not found")
endif(R_INCLUDE_DIR)
if (NOT MSVC)
    add_subdirectory (tests)
endif()
