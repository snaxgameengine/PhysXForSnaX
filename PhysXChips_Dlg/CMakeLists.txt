cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_policy(VERSION 3.15)


file(GLOB_RECURSE PHYSXCHIPS_DLG_SOURCE *.cpp)
file(GLOB_RECURSE PHYSXCHIPS_DLG_HEADER *.h)

add_library(PhysXChips_Dlg SHARED ${PHYSXCHIPS_DLG_SOURCE} ${PHYSXCHIPS_DLG_HEADER})
set_target_properties(PhysXChips_Dlg PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

target_link_libraries(PhysXChips_Dlg PhysXChips)

if (BUILDING_SNAX)
	target_link_libraries(PhysXChips_Dlg ChipDialogs)
else()
	target_link_libraries(PhysXChips_Dlg ${CHIPDIALOGS_LIB})
endif()

add_custom_command(
    TARGET PhysXChips_Dlg 
    POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:PhysXChips_Dlg>
        ${SNAX_BUILD_DIR}/Dialogs/
)