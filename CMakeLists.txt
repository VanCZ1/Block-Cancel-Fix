cmake_minimum_required(VERSION 3.21)
set(NAME "BlockCancelFix")
set(VERSION 1.0.0)

# -------------------------------------------------- #
#                      Settings                      #
# -------------------------------------------------- #
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
option(COPY_BUILD "Copy the build output to the Skyrim directory." ON)

# -------------------------------------------------- #
#                      Project                       #
# -------------------------------------------------- #
project(
	${NAME}
	VERSION ${VERSION}
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.rc
	@ONLY
)

include(cmake/lib/autoAddSources.cmake)

# -------------------------------------------------- #
#                    Dependencies                    #
# -------------------------------------------------- #
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/CommonLibSSE-NG)

find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/utils.hpp")

# -------------------------------------------------- #
#                       Source                       #
# -------------------------------------------------- #
add_src_folder("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_include_folder("${CMAKE_CURRENT_SOURCE_DIR}/include")

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
		${sources}
)

# -------------------------------------------------- #
#                       Build                        #
# -------------------------------------------------- #
add_library(
    ${PROJECT_NAME} 
    SHARED 
		${sources}
		${headers}
		${CMAKE_CURRENT_BINARY_DIR}/Version.rc
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_23
)

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include/PCH.h
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CLIB_UTIL_INCLUDE_DIRS}
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		CommonLibSSE::CommonLibSSE
)

# -------------------------------------------------- #
#                     Post build                     #
# -------------------------------------------------- #
if (COPY_BUILD AND DEFINED ENV{SKYRIM_PATH})
	set(DLL_FOLDER $ENV{SKYRIM_PATH}/Data/SKSE/Plugins)
	
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${DLL_FOLDER}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> ${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>
		VERBATIM
	)
endif ()
