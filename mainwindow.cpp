#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <cmath>  // 确保使用正确的数学库

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , operand("0")  // 初始化显示为0
{
    ui->setupUi(this);

    // 数字按钮连接
    connect(ui->btnNum0, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum1, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum2, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum3, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum4, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum5, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum6, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum7, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum8, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum9, &QPushButton::clicked, this, &MainWindow::btnNumClicked);

    // 双目运算符连接
    connect(ui->btnPlus, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);
    connect(ui->btnMinus, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);
    connect(ui->btnMultiple, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);
    connect(ui->btnDivide, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);

    // 单目运算符连接
    connect(ui->btnPercentage, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);
    connect(ui->btnInverse, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);
    connect(ui->btnSquare, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);
    connect(ui->btnSqrt, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);

    // 初始化显示
    ui->display->setText(operand);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 核心计算函数
QString MainWindow::calculation(bool *ok)
{
    double result = 0;
    if (ok) *ok = false;
    // 处理双目运算
    if (operands.size() >= 2 && !opcodes.isEmpty()) {
        double operand1 = operands[0].toDouble();
        double operand2 = operands[1].toDouble();
        QString op = opcodes[0];

        // 执行运算
        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "*") {
            result = operand1 * operand2;
        } else if (op == "/") {
            if (qFuzzyCompare(operand2, 0.0)) {
                ui->statusbar->showMessage("错误：除数不能为0");
                return "错误";
            }
            result = operand1 / operand2;
        }

        // 清理并保存结果
        operands.clear();
        opcodes.clear();
        operands.push_back(QString::number(result));
        if (ok) *ok = true;
    }
    // 处理单操作数情况
    else if (operands.size() == 1 && opcodes.isEmpty()) {
        result = operands[0].toDouble();
        if (ok) *ok = true;
    } else {
        ui->statusbar->showMessage("计算状态错误");
        return "错误";
    }

    return QString::number(result, 'g', 10);
}

// 数字按钮处理
void MainWindow::btnNumClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString digit = btn->text();
    // 清除初始0
    if (operand == "0" && digit != ".") {
        operand.clear();
    }
    operand += digit;
    ui->display->setText(operand);
}

// 双目运算符处理（+、-、*、/）
void MainWindow::btnBinaryOperatorClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // 保存当前操作数
    if (operand != "0" && !operand.isEmpty()) {
        operands.push_back(operand);
    }

    // 如果已有两个操作数，先计算
    if (operands.size() >= 2) {
        bool ok;
        QString res = calculation(&ok);
        if (ok) {
            ui->display->setText(res);
            operand = res;
        }
    }

    // 保存当前运算符
    QString op = btn->text();
    // 确保乘号按钮文本正确映射为"*"
    if (op == "×" || op == "x" || op == "*") {
        opcodes.push_back("*");
    } else {
        opcodes.push_back(op);
    }

    operand.clear();
}

// 单目运算符处理（%、1/x、x^2、√）
void MainWindow::btnUnaryOperatorClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn || operand.isEmpty() || operand == "0") return;

    double result = operand.toDouble();
    QString op = btn->text();

    // 适配x^2（平方根）和√（开根号）
    if (op == "x^2") {
        result = result * result;
    } else if (op == "√") {
        if (result < 0) {
            ui->statusbar->showMessage("错误：不能对负数开根号");
            ui->display->setText("错误");
            operand = "0";
            return;
        }
        result = sqrt(result);
    } else if (op == "%") {
        result /= 100.0;
    } else if (op == "1/x") {
        if (qFuzzyCompare(result, 0.0)) {
            ui->statusbar->showMessage("错误：0没有倒数");
            ui->display->setText("错误");
            operand = "0";
            return;
        }
        result = 1.0 / result;
    }

    // 更新显示
    operand = QString::number(result, 'g', 10);
    ui->display->setText(operand);
}


// 小数点处理
void MainWindow::on_btnPeriod_clicked()
{
    if (!operand.contains(".")) {
        if (operand.isEmpty()) operand = "0";
        operand += ".";
        ui->display->setText(operand);
    }
}

// 退格处理
void MainWindow::on_btnDel_clicked()
{
    if (operand.length() <= 1) {
        operand = "0";
    } else {
        operand.chop(1);
    }
    ui->display->setText(operand);
}

// 清除所有（AC）
void MainWindow::on_btnClear_clicked()
{
    operand = "0";
    operands.clear();
    opcodes.clear();
    ui->display->setText(operand);
}

// 等于号处理
void MainWindow::on_btnEqual_clicked()
{
    if (operand != "0" && !operand.isEmpty()) {
        operands.push_back(operand);
    }

    bool ok;
    QString result = calculation(&ok);
    if (ok) {
        ui->display->setText(result);
        operand = result;
        operands.clear();
        operands.push_back(operand);
    } else {
        ui->display->setText("错误");
        operand = "0";
    }
}
