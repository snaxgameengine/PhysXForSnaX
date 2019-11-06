cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_policy(VERSION 3.15)

if (NOT BUILDING_SNAX)

	message ("Building PhysX standalone...")

	find_path(SNAX_INSTALL_DIR SnaXDeveloper.exe HINTS "C:/Program Files/SnaX/" DOC "SnaX install directory.")

	if (NOT SNAX_INSTALL_DIR)
		message(WARNING "SnaX installation directory is not found...")
	else()
		SET( SNAX_BUILD_DIR ${SNAX_INSTALL_DIR})
	endif()
	
	add_definitions(-D_UNICODE -DUNICODE)

	project(PhysXForSnaX)

	find_path(SNAX_SDK_DIR Include/M3DCore/MString.h HINTS ./../snaxsdk DOC "Directory of the SnaX SDK.")

	find_library(M3DCORE_LIB M3DCore.lib HINTS ${SNAX_SDK_DIR}/Lib)
	find_library(M3DENGINE_LIB M3DEngine.lib HINTS ${SNAX_SDK_DIR}/Lib)
	find_library(STDCHIPS_LIB StdChips.lib HINTS ${SNAX_SDK_DIR}/Lib)
	find_library(CHIPDIALOGS_LIB ChipDialogs.lib HINTS ${SNAX_SDK_DIR}/Lib)

endif()

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

if (NOT SNAX_SDK_DIR)
	message(FATAL_ERROR "SnaX SDK is not found...")
endif()

find_path(PHYSX_INCLUDE_DIR PhysX/PxPhysics.h)

find_library(PHYSX_LIBRARY PhysX_64.lib)
find_library(PHYSXCOMMON_LIBRARY PhysXCommon_64.lib)
find_library(PHYSXCOOKING_LIBRARY PhysXCooking_64.lib)
find_library(PHYSXFOUNDATION_LIBRARY PhysXFoundation_64.lib)
find_library(PHYSXEXTENSIONS_LIBRARY PhysXExtensions_static_64.lib)
find_library(PHYSXPVDSDK_LIBRARY PhysXPvdSDK_static_64.lib)

find_library(PHYSXGPU_LIBRARY PhysXGpu_64.dll)
find_library(PHYSXDEVICE_LIBRARY PhysXDevice64.dll)

if (NOT PHYSXDEVICE_LIBRARY)
message(WARNING "PhysXDevice64.dll not found. GPU-accelerated PhysX will not be available! If using VCPKG, PhysXDevice64.dll is found somewhere the buildtrees directory.")
endif()

include_directories(.)
include_directories(${PHYSX_INCLUDE_DIR})

add_subdirectory(PhysXChips)
add_subdirectory(PhysXChips_Dlg)

if (NOT BUILDING_SNAX)
add_custom_target(SnaX ALL)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SnaX)
set_property(TARGET SnaX PROPERTY VS_DEBUGGER_COMMAND ${SNAX_INSTALL_DIR}/SnaXDeveloper.exe)
set_property(TARGET SnaX PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${SNAX_INSTALL_DIR})
endif()




