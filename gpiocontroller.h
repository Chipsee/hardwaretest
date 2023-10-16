#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

#include <QObject>
#include "gpiod.h"

class GpioController : public QObject
{
    Q_OBJECT
public:
    explicit GpioController(QObject *parent = nullptr);
    ~GpioController();

    void initializeGpio(const char *chipname, unsigned int offset);
    void setGpioDirection(bool isOutput);
    int readGpioValue();
    void writeGpioValue(int value);

private:
    struct gpiod_chip *chip;
    struct gpiod_line *line;

signals:

};

#endif // GPIOCONTROLLER_H
