/**
 * @file MainWindow.hpp
 * @brief Qt front end, Step 1: a basic arithmetic keypad calling
 * straight into the existing C engine (Inc/calculator.h etc.) --
 * see docs/ROADMAP.md's "Next Up: Phase 31 -- GUI (Qt)".
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class QLineEdit;
class QLabel;

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

private:
    QLineEdit *m_display;
    QLabel *m_errorLabel;
};

#endif
