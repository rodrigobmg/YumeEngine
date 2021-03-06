set(SAMPLE_TARGET "Playground")

set(SOURCE_FILES Playground.cc)

set(HEADER_FILES Playground.h)

if(OS_LINUX OR OS_MACOSX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBOOST_ALL_NO_LIB=1 -g -O0 --coverage")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

include_directories(${YUME_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${YUME_BOOST_PATH})
include_directories(${YUME_3RDPARTY_PATH}/log4cplus/include)
include_directories(${YUME_3RDPARTY_PATH}/assimp/include)
include_directories(${DXUT_INCLUDE_DIRS})

set(COMMON_SOURCES
	${CMAKE_SOURCE_DIR}/Samples/Common/Common.h
	${CMAKE_SOURCE_DIR}/Samples/Common/Common.cc)

if(MSVC)
	add_executable(${SAMPLE_TARGET} WIN32 ${HEADER_FILES} ${SOURCE_FILES} ${COMMON_SOURCES}) #This is to avoid linker error on MSVC so tell that this is a win32 app LOL
endif()
if(NOT MSVC)
	add_executable(${SAMPLE_TARGET} ${HEADER_FILES} ${SOURCE_FILES} ${COMMON_SOURCES})
endif()

target_link_libraries(${SAMPLE_TARGET} ${YUME})
set_target_properties(${SAMPLE_TARGET} PROPERTIES FOLDER "Samples")

source_group(${SAMPLE_TARGET} FILES ${HEADER_FILES} ${SOURCE_FILES})
source_group(Common FILES ${COMMON_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Samples)

set_output_dir(${SAMPLE_TARGET})
