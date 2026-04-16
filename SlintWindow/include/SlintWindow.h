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
#include "Global.h"

class SlintWindow
{
private:
    slint::ComponentHandle<AppWindow> ui_;
    
    UsbCan2EU *usb_can = nullptr;
    void BindEvents();

    void BtnClick_ConnectDevice();
    void BtnClick_DisConnectDevice();
    void BtnClick_StartDiag();
public:
    SlintWindow();
    ~SlintWindow();

    void show();
};
