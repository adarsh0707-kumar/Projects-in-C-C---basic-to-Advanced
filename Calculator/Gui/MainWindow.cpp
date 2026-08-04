#include "MainWindow.hpp"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QString>

#include <cmath>
#include <cstring>

/* Inc/calculator.h, variables.h, and history.h are plain C headers,
   without an extern "C" guard (unlike error.h/complex_eval.h/
   matrix_eval.h, which already have one because they're consumed by
   C++ deliberately). Wrapping the include here -- rather than editing
   every existing C header -- is the standard way to consume a C
   library from C++; see docs/ROADMAP.md's GUI plan. */
extern "C"
{
#include "calculator.h" /* insertImplicitMultiplication, validateExpression,
                            validateParentheses, infixToPostfix,
                            evaluatePostfix, getLastEvalError */
#include "variables.h"  /* setVariable, getVariable, setAns */
#include "history.h"    /* addHistory */
}

namespace
{
/* Matches Src/main.c's buffer sizes as of the 2026-08-04 overflow fix
   (infix/postfix were previously 256, too small for a processed
   buffer that can nearly double the raw input via implicit-
   multiplication expansion) -- new GUI code should not reintroduce
   that bug class. */
constexpr int kExpressionMax = 256;
constexpr int kInfixMax = 512;
constexpr int kPostfixMax = 1024;
constexpr int kVariableNameMax = 32;

QString trimmed(const char *s)
{
    return QString::fromUtf8(s).trimmed();
}
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_display(nullptr), m_errorLabel(nullptr)
{
    setWindowTitle("Scientific Calculator");

    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    m_display = new QLineEdit(central);
    m_display->setMaxLength(kExpressionMax - 1);
    m_display->setAlignment(Qt::AlignRight);
    m_display->setPlaceholderText("0");
    QFont displayFont = m_display->font();
    displayFont.setPointSize(displayFont.pointSize() + 6);
    m_display->setFont(displayFont);
    layout->addWidget(m_display);

    m_errorLabel = new QLabel(central);
    m_errorLabel->setStyleSheet("color: #c0392b;");
    m_errorLabel->setWordWrap(true);
    layout->addWidget(m_errorLabel);

    auto *keypad = new QGridLayout();
    layout->addLayout(keypad);

    struct KeyDef
    {
        const char *label;
        int row, col;
    };

    static const KeyDef valueKeys[] = {
        {"(", 0, 0}, {")", 0, 1}, {"^", 0, 2}, {"%", 0, 3},
        {"7", 1, 0}, {"8", 1, 1}, {"9", 1, 2}, {"/", 1, 3},
        {"4", 2, 0}, {"5", 2, 1}, {"6", 2, 2}, {"*", 2, 3},
        {"1", 3, 0}, {"2", 3, 1}, {"3", 3, 2}, {"-", 3, 3},
        {"0", 4, 0}, {".", 4, 1}, {"!", 4, 2}, {"+", 4, 3},
    };

    const QSizePolicy buttonPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    for (const KeyDef &key : valueKeys)
    {
        auto *button = new QPushButton(QString::fromUtf8(key.label), central);
        button->setSizePolicy(buttonPolicy);
        QString text = QString::fromUtf8(key.label);
        connect(button, &QPushButton::clicked, this, [this, text]()
                { appendToDisplay(text); });
        keypad->addWidget(button, key.row, key.col);
    }

    auto *clearButton = new QPushButton("C", central);
    clearButton->setSizePolicy(buttonPolicy);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearDisplay);
    keypad->addWidget(clearButton, 5, 0);

    auto *backspaceButton = new QPushButton(QStringLiteral("⌫"), central);
    backspaceButton->setSizePolicy(buttonPolicy);
    connect(backspaceButton, &QPushButton::clicked, this, &MainWindow::backspace);
    keypad->addWidget(backspaceButton, 5, 1);

    auto *equalsButton = new QPushButton("=", central);
    equalsButton->setSizePolicy(buttonPolicy);
    connect(equalsButton, &QPushButton::clicked, this, &MainWindow::evaluate);
    keypad->addWidget(equalsButton, 5, 2, 1, 2);

    /* Give every row/column equal weight so the keypad -- not the
       (usually-empty) error label above it -- absorbs any extra
       window space, and buttons actually grow to fill their cells
       instead of sitting at their minimum size with blank space
       around them. */
    for (int row = 0; row <= 5; ++row)
        keypad->setRowStretch(row, 1);
    for (int col = 0; col <= 3; ++col)
        keypad->setColumnStretch(col, 1);

    layout->setStretchFactor(keypad, 1);

    connect(m_display, &QLineEdit::returnPressed, this, &MainWindow::evaluate);

    setCentralWidget(central);
}

void MainWindow::appendToDisplay(const QString &text)
{
    m_errorLabel->clear();
    m_display->insert(text);
}

void MainWindow::clearDisplay()
{
    m_display->clear();
    m_errorLabel->clear();
}

void MainWindow::backspace()
{
    m_display->backspace();
}

void MainWindow::evaluate()
{
    m_errorLabel->clear();

    QByteArray inputBytes = m_display->text().toUtf8();

    if (inputBytes.isEmpty())
        return;

    if (inputBytes.size() >= kExpressionMax)
    {
        m_errorLabel->setText("Error: Expression too long.");
        return;
    }

    char expression[kExpressionMax];
    std::strncpy(expression, inputBytes.constData(), sizeof(expression) - 1);
    expression[sizeof(expression) - 1] = '\0';

    char processed[kInfixMax];
    char infix[kInfixMax];
    char postfix[kPostfixMax];

    insertImplicitMultiplication(expression, processed);
    std::strncpy(infix, processed, sizeof(infix) - 1);
    infix[sizeof(infix) - 1] = '\0';

    char *equalSign = std::strchr(infix, '=');

    if (equalSign != nullptr)
    {
        int nameLen = static_cast<int>(equalSign - infix);

        if (nameLen <= 0)
        {
            m_errorLabel->setText("Error: Missing variable name before '='.");
            return;
        }

        if (nameLen >= kVariableNameMax)
        {
            m_errorLabel->setText("Error: Variable name too long.");
            return;
        }

        char variableName[kVariableNameMax];
        std::strncpy(variableName, infix, nameLen);
        variableName[nameLen] = '\0';

        QString varName = trimmed(variableName);
        QByteArray varNameBytes = varName.toUtf8();

        char rhs[kExpressionMax];
        std::strncpy(rhs, equalSign + 1, sizeof(rhs) - 1);
        rhs[sizeof(rhs) - 1] = '\0';

        insertImplicitMultiplication(rhs, processed);

        if (!validateExpression(processed))
        {
            m_errorLabel->setText("Error: Invalid expression on the right of '='.");
            return;
        }

        if (!validateParentheses(processed))
        {
            m_errorLabel->setText("Error: Mismatched parentheses.");
            return;
        }

        if (!infixToPostfix(processed, postfix))
        {
            m_errorLabel->setText(QString("Error: %1").arg(getLastEvalError()));
            return;
        }

        double result = evaluatePostfix(postfix);

        if (!std::isfinite(result))
        {
            m_errorLabel->setText(QString("Error: %1").arg(getLastEvalError()));
            return;
        }

        setVariable(varNameBytes.constData(), result);
        setAns(result);
        addHistory(infix, result);

        m_display->setText(QString("%1 = %2").arg(varName).arg(result, 0, 'g', 6));
        return;
    }

    if (!validateExpression(infix))
    {
        m_errorLabel->setText("Error: Invalid expression.");
        return;
    }

    if (!validateParentheses(infix))
    {
        m_errorLabel->setText("Error: Mismatched parentheses.");
        return;
    }

    if (!infixToPostfix(infix, postfix))
    {
        m_errorLabel->setText(QString("Error: %1").arg(getLastEvalError()));
        return;
    }

    double result = evaluatePostfix(postfix);

    if (!std::isfinite(result))
    {
        m_errorLabel->setText(QString("Error: %1").arg(getLastEvalError()));
        return;
    }

    setAns(result);
    addHistory(infix, result);

    m_display->setText(QString::number(result, 'g', 6));
}
