#include "gpiocontroller.h"

GpioController::GpioController(QObject *parent)
    : QObject{parent}
{
#ifdef USE_GPIOD_V1
    chip = NULL;
    line = NULL;
#else
    chip = NULL;
    request = NULL;
    settings = NULL;
    line_config = NULL;
    req_config = NULL;

    offset = 0;
    chip_path = NULL;
#endif
}

GpioController::~GpioController()
{
#ifdef USE_GPIOD_V1
    // v1.x release resource
    if (line)
        gpiod_line_release(line);
    if (chip)
        gpiod_chip_close(chip);
#else
    // v2.x release resource
    if (request)
        gpiod_line_request_release(request);
    if (chip)
        gpiod_chip_close(chip);
    if (settings)
        gpiod_line_settings_free(settings);
    if (line_config)
        gpiod_line_config_free(line_config);
    if (req_config)
        gpiod_request_config_free(req_config);
#endif
}

void GpioController::initializeGpio(const char *chipname, unsigned int offset)
{
#ifdef USE_GPIOD_V1
    // v1.x initialize Gpio
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        return;
    }

    line = gpiod_chip_get_line(chip, offset);
    if (!line) {
        return;
    }
#else
    // v2.x initialize Gpio
    this->offset = offset;
    this->chip_path = chipname;

    chip = gpiod_chip_open(this->chip_path);
    if (!chip) {
        return;
    }

    settings = gpiod_line_settings_new();
    if (!settings) {
        return;
    }

    line_config = gpiod_line_config_new();
    if (!line_config) {
        return;
    }

    req_config = gpiod_request_config_new();
    if (!req_config) {
        return;
    }

    gpiod_request_config_set_consumer(req_config, consumer);
#endif
}

struct gpiod_line_request* GpioController::requestInputLine()
{
    if (!chip || !settings || !line_config || !req_config) {
        return NULL;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);

    int ret = gpiod_line_config_add_line_settings(line_config, &offset, 1, settings);
    if (ret) {
        return NULL;
    }

    struct gpiod_line_request *new_request = gpiod_chip_request_lines(chip, req_config, line_config);
    if (!new_request) {
    }

    return new_request;
}

struct gpiod_line_request* GpioController::requestOutputLine(enum gpiod_line_value value)
{
    if (!chip || !settings || !line_config || !req_config) {
        return NULL;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, value);

    int ret = gpiod_line_config_add_line_settings(line_config, &offset, 1, settings);
    if (ret) {
        return NULL;
    }

    struct gpiod_line_request *new_request = gpiod_chip_request_lines(chip, req_config, line_config);
    if (!new_request) {
    }

    return new_request;
}

void GpioController::setGpioDirection(bool isOutput)
{
#ifdef USE_GPIOD_V1
    // v1.x set Gpio Direction
    int ret;
    if (isOutput)
        ret = gpiod_line_request_output(line, "out", 0);
    else
        ret = gpiod_line_request_input(line, "in");

    if (ret < 0) {
        return;
    }
#else
    // v2.x set Gpio Direction
    if (request) {
        gpiod_line_request_release(request);
        request = NULL;
    }

    if (isOutput) {
        request = requestOutputLine(GPIOD_LINE_VALUE_INACTIVE);
    } else {
        request = requestInputLine();
    }

    if (!request) {
        return ;
    }
#endif
}

int GpioController::readGpioValue()
{
#ifdef USE_GPIOD_V1
    // v1.x read value
    if (!line) {
        return -1;
    }
    return gpiod_line_get_value(line);
#else
    // v2.x read value
    if (!request) {
        return GPIOD_LINE_VALUE_ERROR;
    }
    return gpiod_line_request_get_value(request, offset);
#endif
}

void GpioController::writeGpioValue(int value)
{
#ifdef USE_GPIOD_V1
    // v1.x write value
    if (!line) {
        return ;
    }
    gpiod_line_set_value(line, value);
#else
    // v2.x write value
    if (!request) {
        request = requestOutputLine(GPIOD_LINE_VALUE_INACTIVE);
        if (!request) {
            return ;
        }
    }
    enum gpiod_line_value line_value = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    int ret =  gpiod_line_request_set_value(request, offset, line_value);
    if(ret < 0)
        return ;
#endif
}
