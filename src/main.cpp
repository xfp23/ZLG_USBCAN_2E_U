#include "Global.h"

void on_connect()
{
    std::cout << "connect clicked\n";
}

int main() {
    auto ui = AppWindow::create();

    ui->on_connect_clicked(on_connect);

    ui->run();

    return 0;
}