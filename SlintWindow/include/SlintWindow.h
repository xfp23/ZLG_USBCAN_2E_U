/**
 * @file SlintWindow.h
 * @author xfp23
 * @brief 
 * @version 0.1
 * @date 2026-04-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#include "main.h"

class SlintWindow
{
private:
    slint::ComponentHandle<AppWindow> _ui;
public:
    SlintWindow(slint::ComponentHandle<AppWindow> ui);
    ~SlintWindow();
};
