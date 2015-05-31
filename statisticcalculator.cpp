#include <QDebug>
#include <QVector>
#include "statisticcalculator.h"

StatisticCalculator::StatisticCalculator()
    : m_blockSize(1),
      m_groupSize(10.),
      m_tick(0.00001)
{

}

void StatisticCalculator::setValues(const QVector<double>& values) {
    m_values = values;
}

void StatisticCalculator::setBlockSize(double block) {
    m_blockSize = block;
}

void StatisticCalculator::setGroupSeparator(double groupSize)
{
    m_groupSize = groupSize;
}

const QVector<double> &StatisticCalculator::getValues() const
{
    return m_values;
}

const double &StatisticCalculator::getBlockSize() const
{
    return m_blockSize;
}

void StatisticCalculator::calc() {
    m_distribution.clear();

    AvgList<double> pattern;

    QVector<double>::const_iterator it = getValues().end() - 1;
    int n = getBlockSize();
    while (n-- && (it - getValues().begin()) >= 0) {
        pattern.prepend(*it / m_tick);
        it--;
    }

    while ( it - getValues().begin() >= 0 ) {
        m_distribution[*it / m_tick - pattern.average()]++;
        pattern.pop_back();
        pattern.prepend(*it / m_tick);
        it--;
    }
}

void StatisticCalculator::console() {
    QMap<double, double> m = getGrouppedData();
    foreach (double k, m.keys()) {
        qDebug() << QString::number(k, 'f', 10) << ": " << m[k];
    }
}

QMap<double, double> StatisticCalculator::getGrouppedData() const
{
    double step = m_groupSize;
    double wall = step;
    int cnt = 0;
    QMap<double, double> grouped;
    foreach (double k, m_distribution.keys()) {
        if (k > 0) {
            if (k > wall) {
                grouped[wall] = cnt;
                cnt = 0;
                wall += step;
            } else {
                cnt += m_distribution[k];
            }
        }
    }

    wall = -step;
    cnt = 0;
    foreach (double k, reversed(m_distribution.keys())) {
        if (k <= 0) {
            if (k < wall) {
                grouped[wall] = cnt;
                cnt = 0;
                wall -= step;
            } else {
                cnt += m_distribution[k];
            }
        }
    }

    return grouped;
}

QMap<double, double> StatisticCalculator::getRawData() const
{
    return m_distribution;
}
