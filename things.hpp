#include "canvas.hpp"
#include "devices.hpp"

#ifndef THINGS_HPP
#define THINGS_HPP

namespace things {
const short inputScreenPin = 2;
const short clockScreenPin = 3;
const short loadScreenPin = 4;

const short inputDisplayPin = 6;
const short clockDisplayPin = 7;
const short timeDisplayValue = 10;

const short buzzerPin = 9;

const short leftPin = 10;
const short rightPin = 11;
const short topPin = 12;
const short bottomPin = 13;

const short levelPin = A0;
const short brightPin = A1;
const short firstPin = A2;
const short secondPin = A3;

const short period = 25;

devices::screen screen(inputScreenPin, clockScreenPin, loadScreenPin);

devices::display display(inputDisplayPin, clockDisplayPin, timeDisplayValue);

devices::knob level(levelPin);
devices::knob bright(brightPin);
devices::knob first(firstPin);
devices::knob second(secondPin);

devices::button left(leftPin);
devices::button right(rightPin);
devices::button top(topPin);
devices::button bottom(bottomPin);

devices::buzzer buzzer(buzzerPin);

canvas::frame frame;
canvas::layer layer;
}  // namespace things

#endif
