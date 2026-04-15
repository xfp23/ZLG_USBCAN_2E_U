
add_library(USBCAN_2E_U STATIC
    src/USB_CAN_2E_U.cpp
)

target_include_directories(USBCAN_2E_U PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/USBCAN_E_U
)

# 編譯選項：MSVC 下使用 UTF-8
target_compile_options(USBCAN_2E_U PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# if (CMAKE_SIZEOF_VOID_P EQUAL 8)
#     target_link_libraries(USBCAN_2E_U PUBLIC
#         "${CMAKE_CURRENT_SOURCE_DIR}/Lib/USBCAN_E_U/zlgcan.lib"
#     )
# else()
    target_link_libraries(USBCAN_2E_U PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Lib/USBCAN_E_U/zlgcan.lib"
    )
# endif()
