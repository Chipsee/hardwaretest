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

#ifdef USE_GPIOD_V1
    // v1.x
    struct gpiod_chip *chip;
    struct gpiod_line *line;
#else
    // v2.x
    struct gpiod_chip *chip;
    struct gpiod_line_request *request;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_config;
    struct gpiod_request_config *req_config;
    unsigned int offset;
    const char *chip_path;
    const char *consumer = "qt-app";

    struct gpiod_line_request* requestInputLine();
    struct gpiod_line_request* requestOutputLine(enum gpiod_line_value value);
#endif
signals:

};

#endif // GPIOCONTROLLER_H
