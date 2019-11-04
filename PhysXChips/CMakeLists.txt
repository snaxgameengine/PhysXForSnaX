cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_policy(VERSION 3.15)

include_directories(${SNAX_SDK_DIR}/Include)
include_directories(${PHYSX_INCLUDE_DIR})

file(GLOB_RECURSE PHYSXCHIPS_SOURCE *.cpp)
file(GLOB_RECURSE PHYSXCHIPS_HEADER *.h)

add_library(PhysXChips SHARED ${PHYSXCHIPS_SOURCE} ${PHYSXCHIPS_HEADER})
set_target_properties(PhysXChips PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

target_link_libraries(PhysXChips ${M3DCORE_LIB})
target_link_libraries(PhysXChips ${M3DENGINE_LIB})
target_link_libraries(PhysXChips ${STDCHIPS_LIB})
target_link_libraries(PhysXChips ${PHYSX_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXCOMMON_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXCOOKING_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXFOUNDATION_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXEXTENSIONS_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXPVDSDK_LIBRARY})