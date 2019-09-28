cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_policy(VERSION 3.15)

find_path(SNAX_INSTALL_DIR SnaXDeveloper.exe HINTS "C:/Program Files/SnaX/" DOC "SnaX install directory.")

if (NOT SNAX_INSTALL_DIR)
message(WARNING "SnaX installation directory is not found...")
endif()

add_definitions(-D_UNICODE -DUNICODE)


project(PhysXForSnaX)

find_path(SNAX_SDK_DIR Include/M3DCore/MString.h HINTS ./../snaxsdk DOC "Directory of the SnaX SDK.")

if (NOT SNAX_SDK_DIR)
message(FATAL_ERROR "SnaX SDK is not found...")
endif()


find_path(PHYSX_INCLUDE_DIR PhysX/PxPhysics.h)

find_library(M3DCORE_LIB M3DCore.lib HINTS ${SNAX_SDK_DIR}/Lib)
find_library(M3DENGINE_LIB M3DEngine.lib HINTS ${SNAX_SDK_DIR}/Lib)
find_library(STDCHIPS_LIB StdChips.lib HINTS ${SNAX_SDK_DIR}/Lib)
find_library(CHIPDIALOGS_LIB ChipDialogs.lib HINTS ${SNAX_SDK_DIR}/Lib)

find_library(PHYSX_LIBRARY PhysX_64.lib)
find_library(PHYSXCOMMON_LIBRARY PhysXCommon_64.lib)
find_library(PHYSXCOOKING_LIBRARY PhysXCooking_64.lib)
find_library(PHYSXFOUNDATION_LIBRARY PhysXFoundation_64.lib)
find_library(PHYSXEXTENSIONS_LIBRARY PhysXExtensions_static_64.lib)
find_library(PHYSXPVDSDK_LIBRARY PhysXPvdSDK_static_64.lib)



file(GLOB_RECURSE PHYSXCHIPS_SOURCE ./PhysXChips/*.cpp)
file(GLOB_RECURSE PHYSXCHIPS_HEADER ./PhysXChips/*.h)

include_directories(${SNAX_SDK_DIR}/Include)
include_directories(${PHYSX_INCLUDE_DIR})

add_library(PhysXChips SHARED ${PHYSXCHIPS_SOURCE} ${PHYSXCHIPS_HEADER})
set_target_properties(PhysXChips PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(PhysXChips/pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

target_link_libraries(PhysXChips ${M3DCORE_LIB})
target_link_libraries(PhysXChips ${M3DENGINE_LIB})
target_link_libraries(PhysXChips ${STDCHIPS_LIB})
target_link_libraries(PhysXChips ${PHYSX_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXCOMMON_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXCOOKING_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXFOUNDATION_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXEXTENSIONS_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXPVDSDK_LIBRARY})



file(GLOB_RECURSE PHYSXCHIPS_DLG_SOURCE ./PhysXChips_Dlg/*.cpp)
file(GLOB_RECURSE PHYSXCHIPS_DLG_HEADER ./PhysXChips_Dlg/*.h)

include_directories(.)

add_library(PhysXChips_Dlg SHARED ${PHYSXCHIPS_DLG_SOURCE} ${PHYSXCHIPS_DLG_HEADER})
set_target_properties(PhysXChips_Dlg PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(PhysXChips_Dlg/pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

target_link_libraries(PhysXChips_Dlg PhysXChips)
target_link_libraries(PhysXChips_Dlg ${CHIPDIALOGS_LIB})

if (SNAX_INSTALL_DIR)

add_custom_target(SnaX ALL)

add_custom_command(
    TARGET SnaX 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:PhysXChips>
        ${SNAX_INSTALL_DIR}/Chips/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:PhysXChips_Dlg>
        ${SNAX_INSTALL_DIR}/Dialogs/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysX_64.dll
        ${SNAX_INSTALL_DIR}/3rd/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysXCommon_64.dll
        ${SNAX_INSTALL_DIR}/3rd/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysXCooking_64.dll
        ${SNAX_INSTALL_DIR}/3rd/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysXFoundation_64.dll
        ${SNAX_INSTALL_DIR}/3rd/
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SnaX)
set_property(TARGET SnaX PROPERTY VS_DEBUGGER_COMMAND ${SNAX_INSTALL_DIR}/SnaXDeveloper.exe)
set_property(TARGET SnaX PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${SNAX_INSTALL_DIR})

endif()


