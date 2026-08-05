/**
 * @file MainWindow.hpp
 * @brief Qt front end calling straight into the existing C engine
 * (Inc/calculator.h etc.) -- see docs/ROADMAP.md's "In Progress:
 * Phase 31 -- GUI (Qt)" for the step-by-step plan this follows.
 * Step 1: basic arithmetic keypad. Step 2: history panel.
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class QLineEdit;
class QLabel;
class QListWidget;
class QListWidgetItem;

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

private:
    /** Repopulates the history list from history.c's on-disk log. */
    void refreshHistory();

    QLineEdit *m_display;
    QLabel *m_errorLabel;
    QListWidget *m_historyList;
};

#endif
