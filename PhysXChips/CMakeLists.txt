cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_policy(VERSION 3.15)

file(GLOB_RECURSE PHYSXCHIPS_SOURCE *.cpp)
file(GLOB_RECURSE PHYSXCHIPS_HEADER *.h)

add_library(PhysXChips SHARED ${PHYSXCHIPS_SOURCE} ${PHYSXCHIPS_HEADER})
set_target_properties(PhysXChips PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

if (BUILDING_SNAX)
	target_include_directories(PhysXChips PUBLIC ${SNAX_SDK_DIR})
	target_link_libraries(PhysXChips M3DCore)
	target_link_libraries(PhysXChips M3DEngine)
	target_link_libraries(PhysXChips StdChips)
else()
	target_include_directories(PhysXChips PUBLIC ${SNAX_SDK_DIR}/Include)
	target_link_libraries(PhysXChips ${M3DCORE_LIB})
	target_link_libraries(PhysXChips ${M3DENGINE_LIB})
	target_link_libraries(PhysXChips ${STDCHIPS_LIB})
endif()

target_link_libraries(PhysXChips ${PHYSX_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXCOMMON_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXCOOKING_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXFOUNDATION_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXEXTENSIONS_LIBRARY})
target_link_libraries(PhysXChips ${PHYSXPVDSDK_LIBRARY})

add_custom_command(
    TARGET PhysXChips 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:PhysXChips>
        ${SNAX_BUILD_DIR}/Chips/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysX_64.dll
        ${SNAX_BUILD_DIR}/3rd/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysXCommon_64.dll
        ${SNAX_BUILD_DIR}/3rd/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysXCooking_64.dll
        ${SNAX_BUILD_DIR}/3rd/
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:PhysXChips>/PhysXFoundation_64.dll
        ${SNAX_BUILD_DIR}/3rd/
)

if (PHYSXDEVICE_LIBRARY AND PHYSXGPU_LIBRARY)
add_custom_command(
    TARGET PhysXChips 
    POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
        ${PHYSXGPU_LIBRARY}
        ${SNAX_BUILD_DIR}/3rd/		
	COMMAND ${CMAKE_COMMAND} -E copy
        ${PHYSXDEVICE_LIBRARY}
        ${SNAX_BUILD_DIR}/3rd/		
)
endif()