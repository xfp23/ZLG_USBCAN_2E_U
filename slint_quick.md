这份 README 旨在帮助习惯 C++ 开发的“硬核”程序员快速上手 Slint 语法，设计出像样的上位机界面。

---

# 🚀 Slint 上位机 UI 设计快速入门指南

Slint 不是一种编程语言，它是一张**“智能设计图”**。你只需要在 `.slint` 里描述“长什么样”，然后在 C++ 里描述“做什么”。

## 1. 基础：万物皆“框” (Layouts)
在上位机设计中，最重要的是**排版**。Slint 自动帮你计算坐标，你只需要把组件丢进“架子”里。

* **`VerticalBox`**：里面的东西**从上往下**排。
* **`HorizontalBox`**：里面的东西**从左往右**排。
* **`GridLayout`**：网格布局，适合做复杂的参数配置表。

```slint
import { VerticalBox, HorizontalBox, Button } from "std-widgets.slint";

export component AppWindow inherits Window {
    VerticalBox {
        alignment: start; // 顶部对齐
        spacing: 10px;    // 组件间距

        HorizontalBox { 
            // 这一行可以放多个按钮
            Button { text: "启动"; }
            Button { text: "停止"; }
        }
    }
}
```

---

## 2. 核心组件大本营 (Common Widgets)
使用标准组件前，必须在文件顶部引入：
`import { Button, CheckBox, ComboBox, LineEdit, TextEdit, ProgressBar } from "std-widgets.slint";`

| 组件名称 | Slint 标签 | 关键属性 | 作用 |
| :--- | :--- | :--- | :--- |
| **标签** | `Text` | `text`, `font-size`, `color` | 显示静态或动态文本 |
| **按钮** | `Button` | `text`, `clicked => { ... }` | 触发操作 |
| **输入框** | `LineEdit` | `text`, `placeholder-text` | 单行文本输入（如 IP 地址） |
| **多行文本** | `TextEdit` | `text`, `read-only: true` | 日志显示区域 |
| **下拉框** | `ComboBox` | `model: ["CAN0", "CAN1"]` | 选择端口或波特率 |
| **复选框** | `CheckBox` | `checked`, `text` | 开启/关闭某个功能 |
| **进度条** | `ProgressBar` | `progress: 0.5` (0.0~1.0) | 显示任务进度 |

---

## 3. 高级技巧：手撸一个“指示灯” (Custom Component)
标准库里没有“指示灯”？我们可以用 `Rectangle`（矩形）轻松画一个圆形的灯。

```slint
component Indicator {
    in property <bool> active: false; // 输入属性：亮不亮
    
    Rectangle {
        width: 20px;
        height: 20px;
        border-radius: 10px; // 圆角一半即为圆
        background: root.active ? #00FF00 : #444444; // 亮绿灯或暗灰灯
        border-width: 2px;
        border-color: white;
    }
}
```

---

## 4. 属性绑定：UI 的“灵魂”
Slint 最强的地方在于你可以直接在属性里写逻辑（类似 Excel 公式）。

```slint
// 例子：只有勾选了“协议同意”，按钮才允许点击
CheckBox {
    id: my_check;
    text: "我已阅读说明";
}

Button {
    text: "进入系统";
    enabled: my_check.checked; // 自动绑定，不需要在 C++ 里写 if
}
```

---

## 5. 综合实例：标准上位机框架
这是一个包含：串口选择、波特率、状态指示、日志、发送功能的典型上位机 UI 结构。



```slint
import { Button, VerticalBox, HorizontalBox, LineEdit, ComboBox, TextEdit } from "std-widgets.slint";

export component AppWindow inherits Window {
    title: "ZLG USBCAN 调试工具";
    min-width: 600px;
    min-height: 400px;

    in property <bool> is-connected: false;
    callback send-data(string);

    VerticalBox {
        // 第一行：连接控制
        HorizontalBox {
            Text { text: "通道选择:"; vertical-alignment: center; }
            ComboBox { model: ["CAN_CH0", "CAN_CH1"]; }
            
            // 使用上面自定义的指示灯逻辑
            Rectangle {
                width: 16px; height: 16px; border-radius: 8px;
                background: root.is-connected ? green : red;
            }
            
            Button { 
                text: root.is-connected ? "断开" : "连接";
                primary: true; // 蓝色主按钮
            }
        }

        // 第二行：日志显示
        Text { text: "实时日志:"; }
        TextEdit {
            read-only: true;
            text: "等待设备连接...\n";
        }

        // 第三行：发送指令
        HorizontalBox {
            input := LineEdit { placeholder-text: "输入十六进制指令..."; }
            Button {
                text: "发送";
                clicked => { root.send-data(input.text) }
            }
        }
    }
}
```

---

## 💡 给初学者的 3 条建议
1.  **善用 `id`**：给组件起个 `id: my_input`，你就可以在同一个文件的其他地方通过 `my_input.text` 直接引用它的值。
2.  **C++ 只管变量**：在 `.slint` 里定义好 `property`。C++ 只负责通过 `ui->set_...` 改值，不要尝试在 C++ 里去控制按钮的颜色，应该在 `.slint` 里根据属性自动判断颜色。
3.  **单位**：Slint 里的长度单位是 `px`（像素）或百分比。建议多用布局容器，少写死宽高。

---
*祝你的上位机程序一次编译通过！*