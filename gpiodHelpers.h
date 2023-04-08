#ifndef GPIOD_HELPERS_H
#define GPIOD_HELPERS_H

// Version 1.6 it seems to be
#include <gpiod.h>

#include "errorHelpers.h"

// only gonna macro up what I'm actually using for now,
// main header is gigantic

#define gpiod_chip_open_by_number_(num)\
	ERR_NULL(gpiod_chip_open_by_number(num))

#define gpiod_chip_get_line_(chip, offset)\
	ERR_NULL(gpiod_chip_get_line(chip, offset))

#define gpiod_line_request_output_(line, consumer, default_val)\
	ERR_NEG1(gpiod_line_request_output(line, consumer, default_val))

#define gpiod_line_request_falling_edge_events_(line, consumer)\
	ERR_NEG1(gpiod_line_request_falling_edge_events(line, consumer))

#define gpiod_line_event_get_fd_(line)\
	ERR_NEG1(gpiod_line_event_get_fd(line))

#define gpiod_line_event_read_(line, event)\
	ERR_NEG1(gpiod_line_event_read(line, event))

#define gpiod_line_get_value_(line)\
	ERR_NEG1(gpiod_line_get_value(line))

#define gpiod_line_set_value_(line, value)\
	ERR_NEG1(gpiod_line_set_value(line, value))

#endif // GPIOD_HELPERS_H

