cmake_minimum_required(VERSION 3.21)
project(TMC_Diag LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC: 使用 UTF-8 編碼（設置語言選項）
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>" "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# 或者：
# if(MSVC) add_compile_options("/utf-8") endif()

# Slint: 查找安裝好的 Slint 套件
find_package(Slint REQUIRED)

# 子模組
add_subdirectory(components)

# Windows 下支持資源文件 (ICO 等)
if (WIN32)
    enable_language(RC)
endif()

# 可執行檔：含主程序和 Windows 資源文件
add_executable(TMC_Diag
    src/main.cpp
    ui/resources.rc    # 包含 ICON 的資源腳本
)

# Slint UI 文件
target_link_libraries(TMC_Diag PRIVATE Slint::Slint USBCAN_2E_U)
slint_target_sources(TMC_Diag ui/app-window.slint)

# 包含全局頭文件
target_include_directories(TMC_Diag PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Global/include
)

# Windows GUI 選項：如需 GUI（無控制台），使用 WIN32_EXECUTABLE
# set_target_properties(TMC_Diag PROPERTIES WIN32_EXECUTABLE YES)
