/**
 * @file MainWindow.hpp
 * @brief Qt front end calling straight into the existing C engine
 * (Inc/calculator.h etc.) -- see docs/ROADMAP.md's "In Progress:
 * Phase 31 -- GUI (Qt)" for the step-by-step plan this follows.
 * Step 1: basic arithmetic keypad. Step 2: history panel.
 * Step 3: variable manager panel. Step 4: memory panel.
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class QLineEdit;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    /** Appends @p text to the display (digit/operator/paren buttons). */
    void appendToDisplay(const QString &text);
    /** Clears the display and any shown error. */
    void clearDisplay();
    /** Removes the last character from the display. */
    void backspace();
    /** Evaluates the current display text via the C engine. */
    void evaluate();
    /** Recalls the expression behind a clicked history entry into the display. */
    void recallHistoryItem(QListWidgetItem *item);
    /** Clears history.c's on-disk log and this panel's list. */
    void clearHistoryPanel();
    /** Creates/updates a variable from the name/value fields below the table. */
    void setVariableFromFields();
    /** MS: stores the last evaluated result into memory.c's register. */
    void memoryStoreSlot();
    /** MR: recalls memory.c's register into the display. */
    void memoryRecallSlot();
    /** M+: adds the last evaluated result to memory.c's register. */
    void memoryAddSlot();
    /** M-: subtracts the last evaluated result from memory.c's register. */
    void memorySubtractSlot();
    /** MC: resets memory.c's register to 0. */
    void memoryClearSlot();

private:
    /** Repopulates the history list from history.c's on-disk log. */
    void refreshHistory();
    /** Repopulates the variables table from variables.c's in-memory table. */
    void refreshVariables();
    /** Updates the memory tab's label from memory.c's register. */
    void refreshMemory();

    QLineEdit *m_display;
    QLabel *m_errorLabel;
    QListWidget *m_historyList;
    QTableWidget *m_variablesTable;
    QLineEdit *m_newVarName;
    QLineEdit *m_newVarValue;
    QLabel *m_variablesError;
    QLabel *m_memoryLabel;
    /* MS/M+/M- operate on the most recently evaluated result, mirroring
       main.c's MS behavior (Src/main.c: memoryStore(lastResult)) and
       extending the same convenience to M+/M- -- the CLI instead
       prompts for a fresh value via scanf() for those two, which
       doesn't translate to a modal-free GUI without adding a separate
       input dialog for no real benefit. */
    double m_lastResult;
};

#endif
