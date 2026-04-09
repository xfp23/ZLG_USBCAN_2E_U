# Slint 初学者 README：从零写一个上位机 UI

这份笔记面向第一次接触 Slint 的初学者，目标很明确：看完后可以自己写出常见上位机界面，包括按钮、标签、下拉框、复选框、进度条、指示灯、输入框、显示文本框等，并且知道 C++ 后端怎么读写值、怎么接按钮回调、回调函数应该写在哪里。

---

## 1. Slint 是什么

Slint 是一种**声明式 UI 语言**。你不是像传统 GUI 那样一层层“命令式地创建控件”，而是直接写“界面应该长什么样、怎么绑定数据、用户点了以后发生什么”。

你可以把它理解成两层：

* `.slint`：负责描述界面
* `C++`：负责业务逻辑、串口、CAN、Modbus、文件、网络、设备控制等

Slint 的一个常见模式是：

* UI 里定义属性（property）
* 后端 C++ 通过 `set_xxx()` 改值，通过 `get_xxx()` 读值
* UI 里的按钮、输入框、选项变化，通过 callback 传到 C++

---

## 2. 一个最小的 Slint 文件长什么样

```slint
import { Button, VerticalBox, Text } from "std-widgets.slint";

export component App inherits Window {
    width: 400px;
    height: 240px;

    in-out property<int> counter: 0;
    callback button_pressed;

    VerticalBox {
        spacing: 12px;
        padding: 16px;

        Text {
            text: "当前计数：" + root.counter;
        }

        Button {
            text: "点我";
            clicked => {
                root.button_pressed();
            }
        }
    }
}
```

这个例子里你能看到几个关键点：

* `export component App inherits Window`：定义一个窗口级组件
* `property`：定义状态
* `callback`：定义事件回调
* `Button` 的 `clicked => { ... }`：用户点击按钮后执行
* `root.counter`：访问根组件的属性

---

## 3. 语法基础

### 3.1 import

Slint 的标准控件一般从 `std-widgets.slint` 导入。

```slint
import { Button, Text, LineEdit, CheckBox, ProgressIndicator } from "std-widgets.slint";
```

### 3.2 component

```slint
export component App inherits Window {
    // ...
}
```

* `component`：定义组件
* `export`：让这个组件暴露给后端代码使用
* `inherits Window`：表示它是一个窗口

### 3.3 property

```slint
property<int> count: 0;
property<string> title: "Hello";
property<bool> enabled: true;
```

常见写法：

* `property<T>`：普通属性
* `in property<T>`：只读给外部输入
* `out property<T>`：只输出给外部
* `in-out property<T>`：双向属性，最适合上位机里的“状态值”

### 3.4 callback

```slint
callback save_clicked;
callback value_changed(int);
callback confirm(string, int) -> bool;
```

callback 的作用是：**UI 把事件通知给后端**。

### 3.5 function

```slint
public function double(x: int) -> int {
    return x * 2;
}
```

`function` 更像 UI 内部逻辑。真正要给 C++ 频繁接收事件的，一般优先用 `callback`。

---

## 4. 布局：不要一上来就乱写 x/y

Slint 里可以手动写位置，也可以用布局。做上位机界面时，**强烈建议优先使用布局**。

### 4.1 常见布局

* `VerticalBox`：垂直布局
* `HorizontalBox`：水平布局
* `GridLayout`：网格布局
* `GroupBox`：分组框
* `TabWidget`：分页

示例：

```slint
VerticalBox {
    spacing: 8px;

    Text { text: "标题"; }
    Button { text: "确认"; }
}
```

### 4.2 什么时候用 x/y

当你需要：

* 指示灯
* 自定义仪表
* 简单图形布局
* 精确摆放几个小元素

就可以用 `x`、`y`、`width`、`height`。

```slint
Rectangle {
    x: 20px;
    y: 20px;
    width: 16px;
    height: 16px;
    background: green;
}
```

---

## 5. 常用控件速查

## 5.1 按钮 Button

按钮最常见的写法：

```slint
Button {
    text: "开始";
    clicked => {
        root.start_clicked();
    }
}
```

按钮的作用通常是触发动作，例如：

* 开始/停止
* 发送命令
* 读参数
* 写参数

## 5.2 标签 Text

显示静态或动态文本。

```slint
Text {
    text: "设备状态：" + root.status_text;
}
```

适合：

* 标题
* 说明文字
* 状态展示
* 数值显示

## 5.3 输入框 LineEdit

单行输入框，适合输入：

* IP
* 名称
* 数值字符串
* 参数名

```slint
LineEdit {
    placeholder-text: "请输入参数";
    text <=> root.input_text;
}
```

`<=>` 是双向绑定，用户输入会更新到属性，属性改了也会回到输入框。

## 5.4 多行文本框 TextEdit

适合日志、备注、显示大段内容。

```slint
TextEdit {
    text <=> root.log_text;
    read-only: true;
}
```

常见用途：

* 日志窗口
* 接收数据展示
* 备注编辑

## 5.5 复选框 CheckBox

```slint
CheckBox {
    text: "启用功能";
    checked <=> root.feature_enabled;
}
```

常见用途：

* 打开/关闭某个选项
* 使能功能
* 多选设置

## 5.6 开关 Switch

和 CheckBox 类似，但视觉更像“物理开关”。

```slint
Switch {
    text: "自动模式";
    checked <=> root.auto_mode;
}
```

## 5.7 下拉框 ComboBox

```slint
ComboBox {
    model: ["低速", "中速", "高速"];
    current-index <=> root.speed_index;
}
```

常见用途：

* 波特率
* 模式选择
* 设备类型
* 通道选择

## 5.8 进度条 ProgressIndicator

```slint
ProgressIndicator {
    value: root.progress;
}
```

常见用途：

* 下载进度
* 读写进度
* 任务完成度

## 5.9 指示灯

Slint 没有必要非得单独找一个“灯控件”，最简单的方式就是用 `Rectangle` 或 `Ellipse` 风格的自绘小圆点。

```slint
Rectangle {
    width: 14px;
    height: 14px;
    border-radius: 7px;
    background: root.online ? green : red;
}
```

常见用途：

* 通讯在线 / 离线
* 运行 / 停止
* 报警 / 正常

---

## 6. 属性绑定：界面为什么会自动刷新

Slint 的强项之一就是**属性绑定会自动响应变化**。

```slint
Text {
    text: "当前值：" + root.value;
}
```

只要 `root.value` 变了，`Text` 就会自动刷新。

### 6.1 单向绑定

```slint
text: root.value;
```

表示这个控件显示的内容来自 `root.value`。

### 6.2 双向绑定 `<=>`

```slint
LineEdit {
    text <=> root.name;
}
```

适合输入框、复选框、开关、下拉框这种需要“UI 和状态同步”的控件。

### 6.3 一个重要规则

如果你先做了绑定，后面又在代码里直接赋值，原来的绑定可能会被打断。对于可编辑控件，推荐：

* 用 `<=>` 做双向绑定
* 或者用 `changed` / `edited` 一类回调来接收变化

---

## 7. 典型上位机 UI 结构

一个实用的上位机界面通常这样组织：

```slint
export component App inherits Window {
    width: 900px;
    height: 600px;

    in-out property<string> status_text: "未连接";
    in-out property<bool> online: false;
    in-out property<int> progress: 0;
    in-out property<string> input_text: "";
    in-out property<string> log_text: "";
    in-out property<int> mode_index: 0;

    callback connect_clicked;
    callback send_clicked;
    callback clear_log_clicked;

    // 左侧：参数区
    // 右侧：状态区
}
```

建议你把界面分成几块：

* 顶部：标题栏、连接状态
* 左侧：参数设置区
* 中间：控制区
* 右侧：状态显示区
* 底部：日志区

---

## 8. 后端 C++ 怎么读值、改值

Slint 生成 C++ 头文件后，根组件会变成一个可操作的类。官方文档说明：

* 每个公开属性会生成 `get_<属性名>()` 和 `set_<属性名>()`
* 每个 callback 会生成 `on_<回调名>()`
* 回调也可以用 `invoke_<回调名>()` 主动触发

### 8.1 创建界面

```cpp
#include "app.h"

int main()
{
    auto app = App::create();
    app->run();
}
```

### 8.2 读属性

```cpp
auto value = app->get_counter();
auto name = app->get_user_name();
```

### 8.3 写属性

```cpp
app->set_counter(123);
app->set_user_name(slint::SharedString("Alice"));
```

### 8.4 常见命名规则

如果 Slint 里写的是：

```slint
in-out property<string> user-name;
```

那么在 C++ 里通常会变成：

```cpp
get_user_name()
set_user_name()
```

因为 Slint 的 `-` 在 native 代码里会变成 `_`。

---

## 9. 按钮回调怎么接到 C++

这是最关键的部分之一。

### 9.1 在 Slint 里定义 callback

```slint
export component App inherits Window {
    callback connect_clicked;

    Button {
        text: "连接";
        clicked => {
            root.connect_clicked();
        }
    }
}
```

### 9.2 在 C++ 里绑定回调

```cpp
#include "app.h"

int main()
{
    auto app = App::create();

    app->on_connect_clicked([&]() {
        // 这里写你的后端逻辑
        // 例如：打开串口、发送命令、切换状态、更新UI
        app->set_status_text(slint::SharedString("已点击连接"));
        app->set_online(true);
    });

    app->run();
}
```

### 9.3 回调到底写在哪里

回调**不是写在 Slint 文件外面某个神秘地方**，而是：

1. 在 `.slint` 里先声明 `callback xxx;`
2. 在 C++ 创建组件对象后，用 `on_xxx(...)` 绑定
3. 通常写在 `main()` 里，或者写在你的业务类初始化函数里

也就是说，后端回调一般在这些位置之一：

* `main.cpp`
* `AppController` / `UiController`
* 设备管理类初始化函数里

更推荐的工程写法是：**不要把所有逻辑堆在 `main()`**，而是把界面对象传给控制器类管理。

---

## 10. 一个完整的 C++ 例子

```cpp
#include "app.h"
#include <memory>

class UiController {
public:
    UiController()
    {
        app_ = App::create();

        app_->on_start_clicked([this]() {
            app_->set_status_text(slint::SharedString("开始运行"));
            app_->set_online(true);
            app_->set_progress(0);
        });

        app_->on_send_clicked([this]() {
            auto text = app_->get_input_text();
            // 这里可以把 text 发给串口/CAN/网络
            app_->set_log_text(slint::SharedString("发送内容：") + text);
        });

        app_->on_clear_log_clicked([this]() {
            app_->set_log_text(slint::SharedString(""));
        });
    }

    void run()
    {
        app_->run();
    }

private:
    std::shared_ptr<App> app_;
};

int main()
{
    UiController ui;
    ui.run();
}
```

---

## 11. 回调里能做什么，不能做什么

### 能做

* 更新 UI 状态
* 读输入框内容
* 触发设备操作
* 切换页面状态
* 发送命令

### 建议别直接做太重的事

比如：

* 长时间阻塞串口读写
* 死等设备响应
* 在 UI 线程里做大计算

更稳妥的做法是：

* 回调里只发命令、改状态
* 真正耗时任务丢到工作线程
* 任务完成后再回主线程更新界面

---

## 12. 常用“读写 UI 值”的套路

### 12.1 输入框 -> C++

```slint
LineEdit {
    text <=> root.command_text;
}
```

C++：

```cpp
auto cmd = app->get_command_text();
```

### 12.2 C++ -> 标签

```cpp
app->set_status_text(slint::SharedString("连接成功"));
```

### 12.3 C++ -> 进度条

```cpp
app->set_progress(66);
```

### 12.4 C++ -> 复选框

```cpp
app->set_feature_enabled(true);
```

### 12.5 C++ -> 下拉框

```cpp
app->set_mode_index(2);
```

---

## 13. 建议你优先掌握的 Slint 概念

初学者按这个顺序学最稳：

1. `Window` / `component` / `export`
2. `Text` / `Button` / `LineEdit`
3. `property`
4. `callback`
5. `VerticalBox` / `HorizontalBox` / `GridLayout`
6. `<=>` 双向绑定
7. C++ 的 `get_` / `set_` / `on_`
8. 后端控制器写法

---

## 14. 一个最适合上位机的推荐模板

```slint
import { Button, CheckBox, ComboBox, LineEdit, ProgressIndicator, Text, VerticalBox, HorizontalBox } from "std-widgets.slint";

export component App inherits Window {
    width: 800px;
    height: 480px;

    in-out property<string> status_text: "未连接";
    in-out property<string> input_text: "";
    in-out property<string> log_text: "";
    in-out property<int> progress: 0;
    in-out property<int> mode_index: 0;
    in-out property<bool> enabled: false;
    in-out property<bool> online: false;

    callback connect_clicked;
    callback send_clicked;
    callback clear_log_clicked;

    VerticalBox {
        padding: 12px;
        spacing: 10px;

        HorizontalBox {
            spacing: 10px;
            Text { text: "状态：" + root.status_text; }
            Rectangle {
                width: 12px;
                height: 12px;
                border-radius: 6px;
                background: root.online ? green : red;
            }
        }

        HorizontalBox {
            spacing: 10px;
            Text { text: "模式："; }
            ComboBox {
                model: ["模式A", "模式B", "模式C"];
                current-index <=> root.mode_index;
            }
        }

        HorizontalBox {
            spacing: 10px;
            CheckBox {
                text: "功能使能";
                checked <=> root.enabled;
            }
            LineEdit {
                placeholder-text: "输入要发送的数据";
                text <=> root.input_text;
                width: 280px;
            }
        }

        HorizontalBox {
            spacing: 10px;
            Button {
                text: "连接";
                clicked => { root.connect_clicked(); }
            }
            Button {
                text: "发送";
                clicked => { root.send_clicked(); }
            }
            Button {
                text: "清空日志";
                clicked => { root.clear_log_clicked(); }
            }
        }

        ProgressIndicator {
            value: root.progress;
        }

        TextEdit {
            text <=> root.log_text;
            read-only: true;
            height: 200px;
        }
    }
}
```

---

## 15. 最后给初学者的建议

写 Slint 时，尽量记住一句话：

**UI 负责展示和收集状态，C++ 负责真正做事。**

你只要先把下面这套链路跑通，Slint 就已经能用于大多数上位机项目了：

* Slint 定义属性
* Slint 按钮触发 callback
* C++ 用 `on_xxx()` 接回调
* C++ 用 `set_xxx()` 改 UI
* C++ 用 `get_xxx()` 读 UI
* 输入框、复选框、下拉框尽量用 `<=>` 同步

---

## 16. 一页速记

* `property`：状态
* `callback`：事件
* `function`：内部逻辑
* `Button.clicked => {}`：按钮点击事件
* `text: root.xxx`：单向显示
* `text <=> root.xxx`：双向同步
* `get_xxx()`：C++ 读值
* `set_xxx()`：C++ 改值
* `on_xxx()`：C++ 绑定回调
* `invoke_xxx()`：C++ 主动触发回调
* `VerticalBox / HorizontalBox / GridLayout`：布局

---

## 17. 你下一步可以直接做的事

1. 先写一个只有 `Text + Button + LineEdit` 的小界面
2. 再加 `CheckBox + ComboBox + ProgressIndicator`
3. 最后把 C++ 后端接进去
4. 先跑通“按钮点击 -> C++ 改标签文字”
5. 再跑通“输入框内容 -> C++ 读取 -> 回显到日志框”

这样就能非常快地从“看懂”过渡到“能做项目”。
