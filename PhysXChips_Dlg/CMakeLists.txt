cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_policy(VERSION 3.15)

include_directories(${SNAX_SDK_DIR}/Include)
include_directories(${PHYSX_INCLUDE_DIR})


file(GLOB_RECURSE PHYSXCHIPS_DLG_SOURCE *.cpp)
file(GLOB_RECURSE PHYSXCHIPS_DLG_HEADER *.h)

include_directories(.)

add_library(PhysXChips_Dlg SHARED ${PHYSXCHIPS_DLG_SOURCE} ${PHYSXCHIPS_DLG_HEADER})
set_target_properties(PhysXChips_Dlg PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

target_link_libraries(PhysXChips_Dlg PhysXChips)
target_link_libraries(PhysXChips_Dlg ${CHIPDIALOGS_LIB})
