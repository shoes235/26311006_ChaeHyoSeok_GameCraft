cmake_minimum_required(VERSION 3.20)

project(Shooting LANGUAGES CXX)

if(NOT WIN32 OR NOT MSVC)
    message(FATAL_ERROR "Shooting requires Windows and the MSVC toolchain (CLion: select Visual Studio toolchain).")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Shooting")
set(PACKAGES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/packages")
set(GLC2D_DIR "${PACKAGES_DIR}/glc2d.0.1.0.7/build/native")
set(D3DX_DIR "${PACKAGES_DIR}/Microsoft.DXSDK.D3DX.9.29.952.8/build/native")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${SOURCE_DIR}/*.h")
set(PROJECT_FILES ${SOURCES} ${HEADERS})
source_group(TREE "${SOURCE_DIR}" FILES ${PROJECT_FILES})
set(SOURCE_INCLUDE_DIRS "${SOURCE_DIR}")
foreach(HEADER IN LISTS HEADERS)
    get_filename_component(HEADER_DIR "${HEADER}" DIRECTORY)
    list(APPEND SOURCE_INCLUDE_DIRS "${HEADER_DIR}")
endforeach()
list(REMOVE_DUPLICATES SOURCE_INCLUDE_DIRS)
add_executable(Shooting ${PROJECT_FILES})
target_compile_definitions(Shooting PRIVATE UNICODE _UNICODE _CONSOLE)
target_include_directories(Shooting PRIVATE
    ${SOURCE_INCLUDE_DIRS}
    "${GLC2D_DIR}/include"
    "${D3DX_DIR}/include"
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM_DIR x64)
    set(GLC2D_LIB glc2d_x64)
else()
    set(PLATFORM_DIR x86)
    set(GLC2D_LIB glc2d_win32)
endif()

target_link_directories(Shooting PRIVATE "${GLC2D_DIR}/lib")
target_link_libraries(Shooting PRIVATE
    "$<$<CONFIG:Debug>:${GLC2D_LIB}_debug>"
    "$<$<NOT:$<CONFIG:Debug>>:${GLC2D_LIB}_release>"
    "$<$<CONFIG:Debug>:${D3DX_DIR}/debug/lib/${PLATFORM_DIR}/d3dx9d.lib>"
    "$<$<CONFIG:Debug>:${D3DX_DIR}/debug/lib/${PLATFORM_DIR}/d3dx10d.lib>"
    "$<$<CONFIG:Debug>:${D3DX_DIR}/debug/lib/${PLATFORM_DIR}/d3dx11d.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${D3DX_DIR}/release/lib/${PLATFORM_DIR}/d3dx9.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${D3DX_DIR}/release/lib/${PLATFORM_DIR}/d3dx10.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${D3DX_DIR}/release/lib/${PLATFORM_DIR}/d3dx11.lib>"
)

add_custom_command(TARGET Shooting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_DIR}/rsc" "$<TARGET_FILE_DIR:Shooting>/rsc"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${D3DX_DIR}/$<IF:$<CONFIG:Debug>,debug,release>/bin/${PLATFORM_DIR}"
        "$<TARGET_FILE_DIR:Shooting>"
)


