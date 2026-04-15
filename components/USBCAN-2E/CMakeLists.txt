# 靜態庫 USBCAN_2E_U
add_library(USBCAN_2E_U STATIC
    src/USB_CAN_2E_U.cpp
)

target_include_directories(USBCAN_2E_U PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/USBCAN_E_U
)

# 編譯選項：MSVC 下使用 UTF-8
target_compile_options(USBCAN_2E_U PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 根據架構選擇鏈接正確的 zlgcan 庫
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 位目標，鏈接 64 位庫
    target_link_libraries(USBCAN_2E_U PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Lib/USBCAN_E_U/zlgcan.lib"
    )
else()
    # 32 位目標，鏈接 32 位庫
    target_link_libraries(USBCAN_2E_U PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Lib/USBCAN_E_U/zlgcan.lib"
    )
endif()
