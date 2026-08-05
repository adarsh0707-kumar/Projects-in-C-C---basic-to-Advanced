#include "PlotWidget.hpp"

#include <QPainter>
#include <algorithm>
#include <cmath>
#include <limits>

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent), m_xMin(-10.0), m_xMax(10.0), m_hasData(false)
{
    setMinimumHeight(200);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
}

void PlotWidget::setSamples(const QVector<double> &xs, const QVector<double> &ys,
                             const QVector<bool> &valid, double xMin, double xMax)
{
    m_xs = xs;
    m_ys = ys;
    m_valid = valid;
    m_xMin = xMin;
    m_xMax = xMax;
    m_hasData = true;
    update();
}

void PlotWidget::clearPlot()
{
    m_xs.clear();
    m_ys.clear();
    m_valid.clear();
    m_hasData = false;
    update();
}

void PlotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_hasData || m_xs.isEmpty())
        return;

    double yMin = std::numeric_limits<double>::infinity();
    double yMax = -std::numeric_limits<double>::infinity();
    bool anyValid = false;

    for (int i = 0; i < m_ys.size(); ++i)
    {
        if (!m_valid[i])
            continue;

        anyValid = true;
        yMin = std::min(yMin, m_ys[i]);
        yMax = std::max(yMax, m_ys[i]);
    }

    if (!anyValid)
        return;

    /* Flat line (or a single valid point) -- pad the range so the
       x/y mapping below doesn't divide by zero, same treatment
       Src/plot.c gives this case. */
    if (yMax - yMin < 1e-9)
    {
        yMin -= 1.0;
        yMax += 1.0;
    }

    const int marginLeft = 45;
    const int marginRight = 10;
    const int marginTop = 10;
    const int marginBottom = 25;

    const QRectF plotArea(marginLeft, marginTop,
                           width() - marginLeft - marginRight,
                           height() - marginTop - marginBottom);

    auto toPixel = [&](double x, double y) -> QPointF
    {
        double px = plotArea.left() + (x - m_xMin) / (m_xMax - m_xMin) * plotArea.width();
        double py = plotArea.top() + (yMax - y) / (yMax - yMin) * plotArea.height();
        return QPointF(px, py);
    };

    painter.setPen(QPen(QColor(200, 200, 200)));
    painter.drawRect(plotArea);

    /* Axes (y = 0 and x = 0), drawn only when they fall inside the
       visible range -- mirrors Src/plot.c's own axis logic. */
    painter.setPen(QPen(QColor(150, 150, 150), 1, Qt::DashLine));

    if (0.0 >= yMin && 0.0 <= yMax)
    {
        QPointF left = toPixel(m_xMin, 0.0);
        QPointF right = toPixel(m_xMax, 0.0);
        painter.drawLine(left, right);
    }

    if (0.0 >= m_xMin && 0.0 <= m_xMax)
    {
        QPointF top = toPixel(0.0, yMax);
        QPointF bottom = toPixel(0.0, yMin);
        painter.drawLine(top, bottom);
    }

    /* Axis range labels. */
    painter.setPen(QPen(Qt::black));
    QFont labelFont = painter.font();
    labelFont.setPointSize(labelFont.pointSize() - 1);
    painter.setFont(labelFont);
    painter.drawText(QRectF(0, plotArea.top() - 8, marginLeft - 4, 16),
                      Qt::AlignRight, QString::number(yMax, 'g', 3));
    painter.drawText(QRectF(0, plotArea.bottom() - 8, marginLeft - 4, 16),
                      Qt::AlignRight, QString::number(yMin, 'g', 3));
    painter.drawText(QRectF(plotArea.left() - 15, plotArea.bottom() + 4, 40, 16),
                      Qt::AlignLeft, QString::number(m_xMin, 'g', 3));
    painter.drawText(QRectF(plotArea.right() - 25, plotArea.bottom() + 4, 40, 16),
                      Qt::AlignLeft, QString::number(m_xMax, 'g', 3));

    /* The curve itself: consecutive valid samples are connected with a
       line segment; a gap (either endpoint invalid) is simply skipped,
       exactly the treatment Src/plot.c gives asymptotes like 1/x --
       no line is drawn crossing the discontinuity. */
    painter.setPen(QPen(QColor(41, 128, 185), 2));

    for (int i = 0; i + 1 < m_xs.size(); ++i)
    {
        if (!m_valid[i] || !m_valid[i + 1])
            continue;

        painter.drawLine(toPixel(m_xs[i], m_ys[i]), toPixel(m_xs[i + 1], m_ys[i + 1]));
    }
}
