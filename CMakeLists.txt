cmake_minimum_required(VERSION 2.8)
 
project(ethercattest)

## EEROS
## ////////////////////////////////////////////////////////////////////////////
find_package(EEROS REQUIRED)
add_definitions(-DUSE_ETHERCAT)
include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR})

include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})


## ecmasterlib
## Find package ecmasterlib
message(STATUS "")
message(STATUS "EtherCAT")
message(STATUS "**********************************")
find_package(ecmasterlib REQUIRED)
if (ECMASTERLIB_LIB_DIR)
	message(STATUS "--> ecmasterlib found")
	include_directories("${ECMASTERLIB_INCLUDE_DIR}")
	link_directories("${ECMASTERLIB_LIB_DIR}")
	set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ecmasterlib)
	set(LIBECMASTER "${ECMASTERLIB_INCLUDE_DIR}")
	add_definitions(-DUSE_ETHERCAT)
endif (ECMASTERLIB_LIB_DIR)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")



#configure_package_config_file(
#	ethercatInterfaceElmoConfig.cmake.in
#	${CMAKE_CURRENT_BINARY_DIR}/ecmasterlibConfig.cmake
#	INSTALL_DESTINATION ${LIB_INSTALL_DIR}/ecmasterlib/cmake
#	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
	

include_directories(${CMAKE_CURRENT_LIST_DIR}/interface/)
include_directories(/home/ubuntu/installx86/include/interface/)

 
ADD_LIBRARY(EtherCATInterfaceElmo SHARED
	      interface/EtherCATInterfaceElmo.cpp
) 
				
				
add_executable(exampleStandalone
 								example_standalone/main.cpp
)

add_executable(exampleEeros
 								example_eeros/main_eeros.cpp
 								example_eeros/MyControlSystem.cpp
 								example_eeros/MySafetyProperties.cpp
)

target_link_libraries(EtherCATInterfaceElmo ecmasterlib ${CMAKE_DL_LIBS})
target_link_libraries(exampleStandalone EtherCATInterfaceElmo ecmasterlib ${CMAKE_DL_LIBS})
target_link_libraries(exampleEeros eeros ucl EtherCATInterfaceElmo ecmasterlib ${CMAKE_DL_LIBS})

add_subdirectory(interface)
add_subdirectory(example_standalone)
add_subdirectory(example_eeros)


INSTALL(FILES   ${CMAKE_CURRENT_BINARY_DIR}/libEtherCATInterfaceElmo.so DESTINATION lib)
INSTALL(FILES 	${CMAKE_CURRENT_SOURCE_DIR}/interface/EtherCATInterfaceElmo.hpp
				DESTINATION include)
