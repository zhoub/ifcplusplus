CMAKE_MINIMUM_REQUIRED (VERSION 3.7.2)

IF(NOT WIN32)
    IF(NOT CMAKE_BUILD_TYPE)
        SET(CMAKE_BUILD_TYPE "Release")
    ENDIF()

    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
ENDIF(NOT WIN32)

if(NOT DEFINED USE_OPENMP)
  set(USE_OPENMP True)
endif()

if(USE_OPENMP)
  find_package(OpenMP)
endif()

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/IfcPlusPlus/Debug)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/IfcPlusPlus/${CMAKE_BUILD_TYPE})

ADD_EXECUTABLE(LoadFileWithGeometryExampleConsole 
    ${IFCPP_SOURCE_DIR}/examples/LoadFileWithGeometryExampleConsole/src/main.cpp
)

set_target_properties(LoadFileWithGeometryExampleConsole PROPERTIES DEBUG_POSTFIX "d")

if(OpenMP_CXX_FOUND)
    TARGET_LINK_LIBRARIES(LoadFileWithGeometryExampleConsole optimized IfcPlusPlus OpenMP::OpenMP_CXX debug IfcPlusPlusd )
else(OpenMP_CXX_FOUND)
	TARGET_LINK_LIBRARIES(LoadFileWithGeometryExampleConsole optimized IfcPlusPlus debug IfcPlusPlusd)
endif()

IF(BUILD_STATIC_LIBRARY)
	ADD_DEFINITIONS(-DIFCQUERY_STATIC_LIB)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(LoadFileWithGeometryExampleConsole
    PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external
)

INSTALL(
    TARGETS LoadFileWithGeometryExampleConsole
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)
