#ifndef STATISTICCALCULATOR_H
#define STATISTICCALCULATOR_H

#include <QList>
#include <qmap.h>

template<typename T>
class AvgList : public QList<T> {
public:
    const T average() const {
        if (QList<T>::count())
            return m_average / QList<T>::count();
        else
            return 0;
    }

    void pop_back() {
        m_average -= QList<T>::back();
        QList<T>::pop_back();
    }

    void pop_front() {
        m_average -= QList<T>::front();
        QList<T>::pop_front();
    }

    void append(const T &t) {
        QList<T>::append(t);
        m_average += t;
    }

    void prepend(const T &t) {
        QList<T>::prepend(t);
        m_average += t;
    }

    void clear() {
        m_average = 0;
        QList<T>::clear();
    }

private:
    T m_average;
};

template <typename T>
QList<T> reversed( const QList<T> & in ) {
    QList<T> result;
    result.reserve( in.size() ); // reserve is new in Qt 4.7
    std::reverse_copy( in.begin(), in.end(), std::back_inserter( result ) );
    return result;
}

class StatisticCalculator
{
public:
    StatisticCalculator();

    void setValues(const QVector<double> &values);
    void setBlockSize(double block);
    void setGroupSeparator(double groupSize);
    const QVector<double> &getValues() const;
    const double &getBlockSize() const;

    void calc();
    void console();

    QMap<double, double> getGrouppedData() const;
    QMap<double, double> getRawData() const;

private:
    QVector<double> m_values;
    int m_blockSize;
    double m_groupSize;
    double m_tick;

    QMap<double, double> m_distribution;
};

#endif // STATISTICCALCULATOR_H
