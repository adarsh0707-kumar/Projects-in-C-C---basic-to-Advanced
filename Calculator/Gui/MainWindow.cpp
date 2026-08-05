#include "MainWindow.hpp"
#include "PlotWidget.hpp"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QWidget>
#include <QString>
#include <QVector>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cctype>

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
#include "variables.h"  /* setVariable, getVariable, setAns,
                            getVariableCount, getVariableByIndex */
#include "history.h"    /* addHistory, clearHistory, getHistoryCount,
                            getHistoryLineByNumber, getHistoryExpressionByNumber */
#include "memory.h"     /* memoryStore, memoryRecall, memoryAdd,
                            memorySubtract, memoryClear */
#include "stats.h"      /* evaluateStatsExpression */
#include "units.h"      /* evaluateUnitExpression */
#include "base.h"       /* evaluateBaseExpression */
}

/* complex_eval.h/matrix_eval.h already carry their own extern "C"
   guard internally (they're specifically the C-facing entry point
   into the C++ complex/matrix engine), so they're included outside
   the block above rather than redundantly nested inside it. */
#include "complex_eval.h" /* evaluateComplexExpression */
#include "matrix_eval.h"  /* evaluateMatrixExpression */

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

/* Matches Src/plot.c's PLOT_XMIN/PLOT_XMAX exactly, so the GUI plots
   the same domain the CLI's ASCII plot does. Sample count is much
   higher than plot.c's 61 columns since a QPainter line plot benefits
   from a smoother curve, unlike fixed-width ASCII. */
constexpr double kPlotXMin = -10.0;
constexpr double kPlotXMax = 10.0;
constexpr int kPlotSampleCount = 400;

QString trimmed(const char *s)
{
    return QString::fromUtf8(s).trimmed();
}

bool isDigitKeyLabel(const char *label)
{
    return (label[0] >= '0' && label[0] <= '9') || label[0] == '.';
}

/* Step 7 (polish): color-codes key groups via the "keyType" dynamic
   property + attribute selectors below, rather than five near-
   identical setStyleSheet() calls per button. Deliberately doesn't
   use a blanket "QWidget {...}" rule -- PlotWidget (Step 6) paints
   its own white canvas via QPalette/autoFillBackground, and a
   universal QSS selector matching every QWidget would silently
   override that once any stylesheet is active in the window. */
const char *const kStyleSheet = R"CSS(
QLineEdit {
    background-color: #20222f;
    color: #f2f2fa;
    border: 2px solid #3a3c54;
    border-radius: 10px;
    padding: 8px 12px;
    selection-background-color: #5468ff;
}
QLineEdit:focus {
    border: 2px solid #5468ff;
}

QPushButton {
    background-color: #2d2f45;
    color: #e6e6f0;
    border: none;
    border-radius: 10px;
    padding: 10px;
    font-weight: 600;
}
QPushButton:hover {
    background-color: #3a3d5c;
}
QPushButton:pressed {
    background-color: #21222f;
}

QPushButton[keyType="operator"] {
    background-color: #33456e;
    color: #a9d3ff;
}
QPushButton[keyType="operator"]:hover {
    background-color: #3f5484;
}

QPushButton[keyType="equals"] {
    background-color: #3ecf8e;
    color: #10241b;
    font-size: 15px;
}
QPushButton[keyType="equals"]:hover {
    background-color: #55e0a2;
}

QPushButton[keyType="clear"] {
    background-color: #e0555a;
    color: #2a0a0b;
}
QPushButton[keyType="clear"]:hover {
    background-color: #ea7378;
}

QPushButton[keyType="backspace"] {
    background-color: #e0a355;
    color: #2a1a08;
}
QPushButton[keyType="backspace"]:hover {
    background-color: #eab876;
}

QPushButton[keyType="alpha"] {
    background-color: #262840;
    color: #b6b8d6;
    font-size: 11px;
    font-weight: 500;
    padding: 6px;
}
QPushButton[keyType="alpha"]:hover {
    background-color: #333756;
}

QTabBar::tab {
    background: #21222f;
    color: #9d9fbd;
    padding: 8px 14px;
    border-top-left-radius: 8px;
    border-top-right-radius: 8px;
    margin-right: 2px;
}
QTabBar::tab:selected {
    background: #33456e;
    color: #ffffff;
}
QTabBar::tab:hover {
    background: #2d2f45;
}
QTabWidget::pane {
    border: 1px solid #33354c;
    border-radius: 8px;
    top: -1px;
}

QListWidget, QTableWidget {
    background-color: #20222f;
    color: #e6e6f0;
    border: 1px solid #33354c;
    border-radius: 8px;
    gridline-color: #33354c;
}
QHeaderView::section {
    background-color: #2d2f45;
    color: #cfd0e6;
    border: none;
    padding: 4px;
}

QSplitter::handle {
    background-color: #33354c;
}
QSplitter::handle:hover {
    background-color: #454869;
}
)CSS";
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_display(nullptr), m_errorLabel(nullptr), m_historyList(nullptr),
      m_variablesTable(nullptr), m_newVarName(nullptr), m_newVarValue(nullptr), m_variablesError(nullptr),
      m_memoryLabel(nullptr), m_plotInput(nullptr), m_plotError(nullptr), m_plotWidget(nullptr), m_lastResult(0.0)
{
    setWindowTitle("Scientific Calculator");
    setStyleSheet(QString::fromUtf8(kStyleSheet));

    auto *calculatorPanel = new QWidget(this);
    auto *layout = new QVBoxLayout(calculatorPanel);

    m_display = new QLineEdit(calculatorPanel);
    m_display->setMaxLength(kExpressionMax - 1);
    m_display->setAlignment(Qt::AlignRight);
    m_display->setPlaceholderText("0");
    QFont displayFont = m_display->font();
    displayFont.setPointSize(displayFont.pointSize() + 6);
    m_display->setFont(displayFont);
    layout->addWidget(m_display);

    m_errorLabel = new QLabel(calculatorPanel);
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
        auto *button = new QPushButton(QString::fromUtf8(key.label), calculatorPanel);
        button->setSizePolicy(buttonPolicy);
        button->setProperty("keyType", isDigitKeyLabel(key.label) ? "digit" : "operator");
        QString text = QString::fromUtf8(key.label);
        connect(button, &QPushButton::clicked, this, [this, text]()
                { appendToDisplay(text); });
        keypad->addWidget(button, key.row, key.col);
    }

    auto *clearButton = new QPushButton("C", calculatorPanel);
    clearButton->setSizePolicy(buttonPolicy);
    clearButton->setProperty("keyType", "clear");
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearDisplay);
    keypad->addWidget(clearButton, 5, 0);

    auto *backspaceButton = new QPushButton(QStringLiteral("⌫"), calculatorPanel);
    backspaceButton->setSizePolicy(buttonPolicy);
    backspaceButton->setProperty("keyType", "backspace");
    connect(backspaceButton, &QPushButton::clicked, this, &MainWindow::backspace);
    keypad->addWidget(backspaceButton, 5, 1);

    auto *equalsButton = new QPushButton("=", calculatorPanel);
    equalsButton->setSizePolicy(buttonPolicy);
    equalsButton->setProperty("keyType", "equals");
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

    /* Alpha keyboard: the numeric keypad above has no way to produce
       a letter at all, so typing a function name (sin, sqrt, ...) or
       a variable name needed a physical keyboard. QWERTY layout
       (rather than alphabetical) since it's meant to feel like a
       keyboard; labels are uppercase (matching physical keys) but
       insert lowercase, since every function/variable name in this
       engine is matched case-sensitively in lowercase
       (functions.c/variables.c: strcmp() against "sin", "sqrt", ...). */
    auto *alphaKeypad = new QGridLayout();
    layout->addLayout(alphaKeypad);

    static const char *const alphaRows[] = {"QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"};
    constexpr int kAlphaCols = 10;

    for (int row = 0; row < 3; ++row)
    {
        const char *rowLetters = alphaRows[row];

        for (int col = 0; rowLetters[col] != '\0'; ++col)
        {
            char upper = rowLetters[col];
            char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(upper)));

            auto *button = new QPushButton(QString(QChar(upper)), calculatorPanel);
            button->setSizePolicy(buttonPolicy);
            button->setProperty("keyType", "alpha");

            QString insertText{QChar(lower)};
            connect(button, &QPushButton::clicked, this, [this, insertText]()
                    { appendToDisplay(insertText); });

            alphaKeypad->addWidget(button, row, col);
        }
    }

    for (int col = 0; col < kAlphaCols; ++col)
        alphaKeypad->setColumnStretch(col, 1);
    for (int row = 0; row < 3; ++row)
        alphaKeypad->setRowStretch(row, 1);

    layout->setStretchFactor(alphaKeypad, 1);

    connect(m_display, &QLineEdit::returnPressed, this, &MainWindow::evaluate);

    /* History panel (Step 2): reuses history.c's on-disk log (the
       same file the CLI reads/writes) rather than keeping its own
       in-memory copy, so both front ends always agree on what's in
       history. */
    auto *historyPanel = new QWidget(this);
    auto *historyLayout = new QVBoxLayout(historyPanel);

    auto *historyTitle = new QLabel("History", historyPanel);
    historyLayout->addWidget(historyTitle);

    m_historyList = new QListWidget(historyPanel);
    connect(m_historyList, &QListWidget::itemClicked, this, &MainWindow::recallHistoryItem);
    historyLayout->addWidget(m_historyList);

    auto *clearHistoryButton = new QPushButton("Clear History", historyPanel);
    connect(clearHistoryButton, &QPushButton::clicked, this, &MainWindow::clearHistoryPanel);
    historyLayout->addWidget(clearHistoryButton);

    /* Variable manager panel (Step 3): reuses variables.c's in-memory
       table via the same read-then-display pattern as the history
       panel above. The table itself is read-only -- editing/creating
       a variable goes through the name/value fields and Set button
       below it, which is exactly what setVariable() already supports
       (create-or-update), rather than wiring up in-place cell editing
       (and the signal-blocking that'd need, to avoid refreshVariables()
       triggering itself). */
    auto *variablesPanel = new QWidget(this);
    auto *variablesLayout = new QVBoxLayout(variablesPanel);

    m_variablesTable = new QTableWidget(0, 2, variablesPanel);
    m_variablesTable->setHorizontalHeaderLabels({"Name", "Value"});
    m_variablesTable->horizontalHeader()->setStretchLastSection(true);
    m_variablesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_variablesTable->setSelectionMode(QAbstractItemView::NoSelection);
    variablesLayout->addWidget(m_variablesTable);

    auto *newVarLayout = new QHBoxLayout();
    m_newVarName = new QLineEdit(variablesPanel);
    m_newVarName->setPlaceholderText("name");
    m_newVarName->setMaxLength(kVariableNameMax - 1);
    newVarLayout->addWidget(m_newVarName);

    m_newVarValue = new QLineEdit(variablesPanel);
    m_newVarValue->setPlaceholderText("value");
    newVarLayout->addWidget(m_newVarValue);

    auto *setVarButton = new QPushButton("Set", variablesPanel);
    connect(setVarButton, &QPushButton::clicked, this, &MainWindow::setVariableFromFields);
    newVarLayout->addWidget(setVarButton);

    connect(m_newVarName, &QLineEdit::returnPressed, this, &MainWindow::setVariableFromFields);
    connect(m_newVarValue, &QLineEdit::returnPressed, this, &MainWindow::setVariableFromFields);

    variablesLayout->addLayout(newVarLayout);

    m_variablesError = new QLabel(variablesPanel);
    m_variablesError->setStyleSheet("color: #c0392b;");
    m_variablesError->setWordWrap(true);
    variablesLayout->addWidget(m_variablesError);

    /* Memory panel (Step 4): memory.c is a single register (not a
       list), so unlike Steps 2-3 this needed no new engine-side
       enumeration function -- memoryRecall() already gives the panel
       everything it needs to display. */
    auto *memoryPanel = new QWidget(this);
    auto *memoryLayout = new QVBoxLayout(memoryPanel);

    m_memoryLabel = new QLabel(memoryPanel);
    QFont memoryFont = m_memoryLabel->font();
    memoryFont.setPointSize(memoryFont.pointSize() + 4);
    m_memoryLabel->setFont(memoryFont);
    m_memoryLabel->setAlignment(Qt::AlignCenter);
    memoryLayout->addWidget(m_memoryLabel);

    auto *msButton = new QPushButton("MS", memoryPanel);
    connect(msButton, &QPushButton::clicked, this, &MainWindow::memoryStoreSlot);
    memoryLayout->addWidget(msButton);

    auto *mrButton = new QPushButton("MR", memoryPanel);
    connect(mrButton, &QPushButton::clicked, this, &MainWindow::memoryRecallSlot);
    memoryLayout->addWidget(mrButton);

    auto *mPlusButton = new QPushButton("M+", memoryPanel);
    connect(mPlusButton, &QPushButton::clicked, this, &MainWindow::memoryAddSlot);
    memoryLayout->addWidget(mPlusButton);

    auto *mMinusButton = new QPushButton("M-", memoryPanel);
    connect(mMinusButton, &QPushButton::clicked, this, &MainWindow::memorySubtractSlot);
    memoryLayout->addWidget(mMinusButton);

    auto *mcButton = new QPushButton("MC", memoryPanel);
    connect(mcButton, &QPushButton::clicked, this, &MainWindow::memoryClearSlot);
    memoryLayout->addWidget(mcButton);

    memoryLayout->addStretch(1);

    /* Plot panel (Step 6): real QPainter graphics instead of Step 1-5's
       ASCII-in-a-label approach. Sampling logic mirrors Src/plot.c's
       own pipeline (insertImplicitMultiplication -> validateExpression
       -> validateParentheses -> per-sample infixToPostfix/
       evaluatePostfix with setVariable("x", ...)), but builds (x, y,
       valid) arrays for PlotWidget to draw instead of an ASCII canvas.
       Unlike plot.c's CLI syntax, the expression here is typed bare
       (just "sin(x)", not "plot(sin(x))") since this tab is already
       dedicated to plotting. */
    auto *plotPanel = new QWidget(this);
    auto *plotLayout = new QVBoxLayout(plotPanel);

    m_plotInput = new QLineEdit(plotPanel);
    m_plotInput->setPlaceholderText("e.g. sin(x), x^2, 1/x");
    plotLayout->addWidget(m_plotInput);

    m_plotWidget = new PlotWidget(plotPanel);
    plotLayout->addWidget(m_plotWidget);
    plotLayout->setStretchFactor(m_plotWidget, 1);

    m_plotError = new QLabel(plotPanel);
    m_plotError->setStyleSheet("color: #c0392b;");
    m_plotError->setWordWrap(true);
    plotLayout->addWidget(m_plotError);

    connect(m_plotInput, &QLineEdit::returnPressed, this, &MainWindow::plotCurrentExpression);

    auto *plotButton = new QPushButton("Plot", plotPanel);
    connect(plotButton, &QPushButton::clicked, this, &MainWindow::plotCurrentExpression);
    plotLayout->addWidget(plotButton);

    auto *sidePanel = new QTabWidget(this);
    sidePanel->addTab(historyPanel, "History");
    sidePanel->addTab(variablesPanel, "Variables");
    sidePanel->addTab(memoryPanel, "Memory");

    /* Step 5: five thin evaluate*Expression() tabs, all built by the
       one buildEvalTab() helper since they share an identical
       function signature (see MainWindow.hpp's EvalFn). */
    sidePanel->addTab(buildEvalTab(evaluateComplexExpression, "e.g. (2+3i)*(4-5i), sqrt(-1)"), "Complex");
    sidePanel->addTab(buildEvalTab(evaluateMatrixExpression, "e.g. det([[1,2],[3,4]])"), "Matrix");
    sidePanel->addTab(buildEvalTab(evaluateStatsExpression, "e.g. mean(1,2,3,4), stddev(4,8,6,5,3,7)"), "Statistics");
    sidePanel->addTab(buildEvalTab(evaluateUnitExpression, "e.g. 10km, 10km to miles, 80F to C"), "Units");
    sidePanel->addTab(buildEvalTab(evaluateBaseExpression, "e.g. bin(25), hex(255), dec(FFh)"), "Base");
    sidePanel->addTab(plotPanel, "Plot");

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(calculatorPanel);
    splitter->addWidget(sidePanel);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);

    refreshHistory();
    refreshVariables();
    refreshMemory();
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
        m_lastResult = result;
        refreshHistory();
        refreshVariables();

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
    m_lastResult = result;
    refreshHistory();
    refreshVariables();

    m_display->setText(QString::number(result, 'g', 6));
}

void MainWindow::refreshHistory()
{
    m_historyList->clear();

    int count = getHistoryCount();
    char line[256];

    for (int n = 1; n <= count; ++n)
    {
        if (getHistoryLineByNumber(n, line, sizeof(line)))
            m_historyList->addItem(QString::fromUtf8(line));
    }

    m_historyList->scrollToBottom();
}

void MainWindow::recallHistoryItem(QListWidgetItem *item)
{
    if (item == nullptr)
        return;

    int n = m_historyList->row(item) + 1;
    char expr[256];

    if (!getHistoryExpressionByNumber(n, expr, sizeof(expr)))
        return;

    m_errorLabel->clear();
    m_display->setText(QString::fromUtf8(expr));
}

void MainWindow::clearHistoryPanel()
{
    clearHistory();
    refreshHistory();
}

void MainWindow::refreshVariables()
{
    m_variablesTable->setRowCount(0);

    int count = getVariableCount();
    char name[kVariableNameMax];
    double value;

    for (int i = 0; i < count; ++i)
    {
        if (!getVariableByIndex(i, name, sizeof(name), &value))
            continue;

        int row = m_variablesTable->rowCount();
        m_variablesTable->insertRow(row);
        m_variablesTable->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(name)));
        m_variablesTable->setItem(row, 1, new QTableWidgetItem(QString::number(value, 'g', 6)));
    }
}

void MainWindow::setVariableFromFields()
{
    m_variablesError->clear();

    QString name = m_newVarName->text().trimmed();

    if (name.isEmpty())
    {
        m_variablesError->setText("Error: Enter a variable name.");
        return;
    }

    /* Matches the identifier rule the expression parser itself uses
       (postfix.c: starts with a letter, then letters/digits/'_') --
       a variable this panel creates has to actually be usable when
       typed into an expression, not just storable. */
    QByteArray nameBytes = name.toUtf8();
    const char *nameData = nameBytes.constData();

    if (!std::isalpha(static_cast<unsigned char>(nameData[0])))
    {
        m_variablesError->setText("Error: A variable name must start with a letter.");
        return;
    }

    for (int i = 1; i < nameBytes.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(nameData[i]);
        if (!std::isalnum(c) && c != '_')
        {
            m_variablesError->setText("Error: A variable name may only contain letters, digits, and '_'.");
            return;
        }
    }

    bool ok = false;
    double value = m_newVarValue->text().trimmed().toDouble(&ok);

    if (!ok)
    {
        m_variablesError->setText("Error: Enter a valid numeric value.");
        return;
    }

    if (!setVariable(nameData, value))
    {
        m_variablesError->setText(QString("Error: '%1' is a read-only constant, or the variable table is full.").arg(name));
        return;
    }

    m_newVarName->clear();
    m_newVarValue->clear();
    refreshVariables();
}

void MainWindow::refreshMemory()
{
    m_memoryLabel->setText(QString("Memory: %1").arg(memoryRecall(), 0, 'g', 6));
}

void MainWindow::memoryStoreSlot()
{
    memoryStore(m_lastResult);
    refreshMemory();
}

void MainWindow::memoryRecallSlot()
{
    m_errorLabel->clear();
    m_display->setText(QString::number(memoryRecall(), 'g', 6));
}

void MainWindow::memoryAddSlot()
{
    memoryAdd(m_lastResult);
    refreshMemory();
}

void MainWindow::memorySubtractSlot()
{
    memorySubtract(m_lastResult);
    refreshMemory();
}

void MainWindow::memoryClearSlot()
{
    memoryClear();
    refreshMemory();
}

void MainWindow::plotCurrentExpression()
{
    m_plotError->clear();

    QByteArray exprBytes = m_plotInput->text().toUtf8();

    if (exprBytes.isEmpty())
    {
        m_plotWidget->clearPlot();
        return;
    }

    if (exprBytes.size() >= kExpressionMax)
    {
        m_plotError->setText("Error: Expression too long.");
        m_plotWidget->clearPlot();
        return;
    }

    char expression[kExpressionMax];
    std::strncpy(expression, exprBytes.constData(), sizeof(expression) - 1);
    expression[sizeof(expression) - 1] = '\0';

    char processed[kInfixMax];
    insertImplicitMultiplication(expression, processed);

    /* Syntax only needs checking once -- it doesn't depend on x's
       value, exactly as Src/plot.c's own comment notes. */
    if (!validateExpression(processed))
    {
        m_plotError->setText("Error: Invalid expression.");
        m_plotWidget->clearPlot();
        return;
    }

    if (!validateParentheses(processed))
    {
        m_plotError->setText("Error: Mismatched parentheses.");
        m_plotWidget->clearPlot();
        return;
    }

    QVector<double> xs(kPlotSampleCount);
    QVector<double> ys(kPlotSampleCount);
    QVector<bool> valid(kPlotSampleCount);

    for (int i = 0; i < kPlotSampleCount; ++i)
    {
        double x = kPlotXMin + i * (kPlotXMax - kPlotXMin) / (kPlotSampleCount - 1);
        setVariable("x", x);

        char postfix[kPostfixMax];

        if (!infixToPostfix(processed, postfix))
        {
            /* A structural failure (undefined variable, malformed
               number, ...) independent of x's value -- identical on
               every sample, so stop instead of repeating it 400
               times, same reasoning as Src/plot.c. */
            m_plotError->setText(QString("Error: %1").arg(getLastEvalError()));
            m_plotWidget->clearPlot();
            return;
        }

        double y = evaluatePostfix(postfix);

        xs[i] = x;
        ys[i] = y;
        valid[i] = std::isfinite(y);
    }

    bool anyValid = std::any_of(valid.cbegin(), valid.cend(), [](bool v)
                                 { return v; });

    if (!anyValid)
    {
        m_plotError->setText(QString("Error: '%1' produced no finite values for x in [%2, %3].")
                                  .arg(m_plotInput->text())
                                  .arg(kPlotXMin, 0, 'g', 3)
                                  .arg(kPlotXMax, 0, 'g', 3));
        m_plotWidget->clearPlot();
        return;
    }

    m_plotWidget->setSamples(xs, ys, valid, kPlotXMin, kPlotXMax);
}

QWidget *MainWindow::buildEvalTab(EvalFn evalFn, const QString &placeholderText)
{
    auto *panel = new QWidget(this);
    auto *panelLayout = new QVBoxLayout(panel);

    auto *input = new QLineEdit(panel);
    input->setPlaceholderText(placeholderText);
    panelLayout->addWidget(input);

    auto *resultLabel = new QLabel(panel);
    resultLabel->setWordWrap(true);
    resultLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    panelLayout->addWidget(resultLabel);

    /* A local lambda (not a declared slot) works fine here since
       Qt's functor-based connect() doesn't need MOC-generated
       machinery for the connection itself -- only Q_OBJECT/signals/
       slots that cross to other objects need that. This keeps
       MainWindow.hpp from needing five near-identical slot
       declarations for what's really one behavior parameterized by
       which C function to call. */
    auto runEval = [input, resultLabel, evalFn]()
    {
        QByteArray exprBytes = input->text().toUtf8();
        char result[512] = "";

        int ok = evalFn(exprBytes.constData(), result, sizeof(result));

        resultLabel->setStyleSheet(ok ? "" : "color: #c0392b;");
        resultLabel->setText(QString::fromUtf8(result));
    };

    connect(input, &QLineEdit::returnPressed, this, runEval);

    auto *evalButton = new QPushButton("Evaluate", panel);
    connect(evalButton, &QPushButton::clicked, this, runEval);
    panelLayout->addWidget(evalButton);

    panelLayout->addStretch(1);

    return panel;
}
