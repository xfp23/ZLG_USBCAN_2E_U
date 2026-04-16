#include "SlintWindow.h"
// #include "Global.h"

SlintWindow::SlintWindow() : ui_(AppWindow::create()) 
{
    this->BindEvents();
    this->usb_can = new UsbCan2EU();

}

SlintWindow::~SlintWindow()
{
    // delete this->usb_can;
}

// Private:
void SlintWindow::BindEvents()
{
    // auto ui = this->ui_;

    ui_->on_connect_clicked([this]{BtnClick_ConnectDevice();});
    ui_->on_disconnect_clicked([this]{BtnClick_DisConnectDevice();});
    ui_->on_start_test_clicked([this]{BtnClick_StartDiag();});
}

void SlintWindow::show()
{
    this->ui_->run();
}
void SlintWindow::BtnClick_ConnectDevice()
{
    if(usb_can->OpenDev() != UsbCan2EU::Status_t::OK)
    {
            this->ui_->set_OpenDevError(true);
            return;
    }
    int ch = ui_->get_selected_can_channel();
    int baud = ui_->get_selected_can_baud();
    if(usb_can->OpenChannel((UsbCan2EU::Channel_t)ch,(UsbCan2EU::BaudRate_t)baud) != UsbCan2EU::Status_t::OK)
    {
        this->ui_->set_OpenDevError(true);
        return;
    }

        this->ui_->set_OpenDevBtnEnable(false);
        this->ui_->set_CloseDevBtnEnable(true);
        this->ui_->set_StartDiagBtnEnable(true);
}

void SlintWindow::BtnClick_DisConnectDevice()
{
    this->ui_->set_OpenDevBtnEnable(true);
    this->ui_->set_CloseDevBtnEnable(false);
    this->ui_->set_StartDiagBtnEnable(false);
}

void SlintWindow::BtnClick_StartDiag()
{

}
