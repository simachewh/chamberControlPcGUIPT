#include "chamber.h"

Chamber::Chamber(QObject *parent) : QObject(parent)
{

}

Chamber::~Chamber(){

}

//! ********** Getters and Setters ********* !//

/**
 * @brief Chamber::getDryTemperature function.
 * Gets dryTemperature property.
 * @return double dryTemperature.
 */
double Chamber::getDryTemperature(){
    return dryTemperature;
}

/**
 * @brief Chamber::getWetTemperature function.
 * Gets wetTemperature property.
 * @return double wetTemperature.
 */
double Chamber::getWetTemperature(){
    return wetTemperature;
}

/**
 * @brief Chamber::getHumidity function.
 * Gets humidity property.
 * @return double humidity.
 */
double Chamber::getHumidity(){
    return humidity;
}

void Chamber::setDryTemprature(double value){
    if(getDryTemperature() != value){
        dryTemperature = value;
        emit dryTemperatureChanged(QString("%1").arg(dryTemperature, 6, 'f', 2, '0'));
        emit dryTemperatureChanged(value);
    }
}

void Chamber::setWetTemprature(double value){
    if(getWetTemperature() != value){
        wetTemperature = value;
        emit wetTemperatureChanged(QString("%1").arg(wetTemperature, 6, 'f', 2, '0'));
        emit wetTemperatureChanged(value);
    }
}

void Chamber::setHumidity(double value){
    if(getHumidity() != value){
        humidity = value;
        emit humidityChanged(QString("%1").arg(humidity, 4, 'f', 1, '0'));
        emit humidityChanged(value);
    }
}

