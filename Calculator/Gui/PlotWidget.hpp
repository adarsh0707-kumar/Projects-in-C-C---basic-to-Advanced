/**
 * @file PlotWidget.hpp
 * @brief Pure-rendering QPainter plot area for Step 6 of docs/ROADMAP.md's
 * GUI plan. Deliberately has no dependency on the calculator engine at
 * all -- MainWindow does the sampling (mirroring Src/plot.c's own
 * insertImplicitMultiplication -> validate -> infixToPostfix ->
 * evaluatePostfix pipeline) and hands this widget plain (x, y, valid)
 * arrays to draw. No Q_OBJECT: it has no signals/slots of its own, just
 * a public setter and a paintEvent() override, so it needs no moc step.
 */
#ifndef PLOTWIDGET_HPP
#define PLOTWIDGET_HPP

#include <QWidget>
#include <QVector>

class PlotWidget : public QWidget
{
public:
    explicit PlotWidget(QWidget *parent = nullptr);

    /** Replaces the current curve with a new set of samples and repaints.
     *  @p valid marks samples to skip (non-finite results), which draws
     *  as a gap in the curve rather than a wrong point or a crash --
     *  the same treatment Src/plot.c gives asymptotes like 1/x. */
    void setSamples(const QVector<double> &xs, const QVector<double> &ys,
                     const QVector<bool> &valid, double xMin, double xMax);

    /** Empties the plot area (shown when there's nothing valid to draw). */
    void clearPlot();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> m_xs;
    QVector<double> m_ys;
    QVector<bool> m_valid;
    double m_xMin;
    double m_xMax;
    bool m_hasData;
};

#endif
