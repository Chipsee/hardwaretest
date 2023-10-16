#include "gpiocontroller.h"

GpioController::GpioController(QObject *parent)
    : QObject{parent}
{
    chip = NULL;
    line = NULL;
}

GpioController::~GpioController()
{
    if (line)
        gpiod_line_release(line);
    if (chip)
        gpiod_chip_close(chip);
}

void GpioController::initializeGpio(const char *chipname, unsigned int offset)
{
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        return;
    }

    line = gpiod_chip_get_line(chip, offset);
    if (!line) {
        return;
    }
}

void GpioController::setGpioDirection(bool isOutput)
{
    int ret;
    if (isOutput)
        ret = gpiod_line_request_output(line, "out", 0);
    else
        ret = gpiod_line_request_input(line, "in");

    if (ret < 0) {
        return;
    }
}

int GpioController::readGpioValue()
{
    return gpiod_line_get_value(line);
}

void GpioController::writeGpioValue(int value)
{
    gpiod_line_set_value(line, value);
}
