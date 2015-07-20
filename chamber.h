#ifndef CHAMBER_H
#define CHAMBER_H

#include <QWidget>
/**
 * @brief The Chamber class.
 * This class represents the climate chamber, it holds properties to represnts variables
 * in the chanber's state.
 */
class Chamber : public QObject
{
    Q_OBJECT
private:
    double dryTemperature;
    double wetTemperature;
    double humidity;
    
public:
    enum command {ACK, B, A, I};
    double getDryTemperature();
    double getWetTemperature();
    double getHumidity();
    
    explicit Chamber(QObject *parent = 0);
    ~Chamber();

signals:
    void dryTemperatureChanged(QString value);
    void wetTemperatureChanged(QString value);
    void humidityChanged(QString value);
    void chCommandChanged(Chamber::command chCommand);

public slots:
    void setDryTemprature(double value);
    void setWetTemprature(double value);
    void setHumidity(double value);
};

#endif // CHAMBER_H
