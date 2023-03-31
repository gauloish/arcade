#include "canvas.hpp"
#include "utils.hpp"

#ifndef DEVICES_HPP
#define DEVICES_HPP

namespace devices {
class screen {
   private:
    const int input;
    const int clock;
    const int load;

    const byte noop{0x0};
    const byte mode{0x9};
    const byte intensity{0xA};
    const byte limit{0xB};
    const byte shutdown{0xC};
    const byte test{0xF};

   public:
    screen(int input, int clock, int load) : input(input), clock(clock), load(load) {
        pinMode(input, OUTPUT);
        pinMode(clock, OUTPUT);
        pinMode(load, OUTPUT);

        control(mode, 0x00);       // decode mode
        control(test, 0x00);       // display test
        control(limit, 0x07);      // scan limit
        control(shutdown, 0x01);   // shutdown
        control(intensity, 0x08);  // intensity

        clear();
    }

    void control(byte adress, byte data) const {
        digitalWrite(load, false);
        shiftOut(input, clock, MSBFIRST, adress);
        shiftOut(input, clock, MSBFIRST, data);
        digitalWrite(load, true);
    }

    void render(const canvas::frame &pattern) const {
        short index = short();

        for (const short &line : pattern) {
            control(++index, numerics::reverse(line));
        }
    }

    void render(const canvas::frame &&pattern) const { render(pattern); }

    void clear() const { render(canvas::frame()); }

    void brightness(byte bright) const {
        bright = map(bright, 0, 100, 1, 15);

        control(intensity, bright);
    }
};

class display {
   private:
    const int input;
    const int clock;
    const int time;

    const int first{0x40};
    const int second{0xC0};
    const int third{0x80};

    bool dots{false};

   public:
    display(int input, int clock, int time) : input(input), clock(clock), time(time) {
        pinMode(input, INPUT);
        pinMode(clock, INPUT);

        digitalWrite(input, LOW);
        digitalWrite(clock, LOW);
    }

    void wait() const { delayMicroseconds(time); }

    void start() const {
        pinMode(input, OUTPUT);
        wait();
    }

    void stop() const {
        pinMode(input, OUTPUT);
        wait();
        pinMode(clock, INPUT);
        wait();
        pinMode(input, INPUT);
        wait();
    }

    void write(byte data) const {
        byte condition = 8;

        while (condition--) {
            pinMode(clock, OUTPUT);
            wait();

            if (data & 1)
                pinMode(input, INPUT);
            else
                pinMode(input, OUTPUT);

            wait();
            pinMode(clock, INPUT);
            wait();

            data = data >> 1;
        }

        pinMode(clock, OUTPUT);
        pinMode(input, INPUT);
        wait();

        pinMode(clock, INPUT);
        wait();

        bool value = digitalRead(input);

        if (value) {
            pinMode(input, OUTPUT);
        }

        wait();
        pinMode(clock, OUTPUT);
        wait();
    }

    void render(const canvas::layer &segments) const {
        start();
        write(first);
        stop();

        start();
        write(second);

        for (short &segment : segments) {
            byte data = numerics::encode(segment);

            if (dots) {
                data = data | (1 << 7);
            }

            write(data);
        }

        stop();
    }

    void render(const canvas::layer &&segments) const { render(segments); }

    void clear() const { render(canvas::layer()); }

    void separator(bool value) { dots = value; }

    void brightness(byte bright, bool enable = true) const {
        bright = map(bright, 0, 100, 0, 7);

        if (enable) {
            bright = ((bright & 0b0111) | 0b1000) & 0b1111;
        } else {
            bright = ((bright & 0b0111) | 0b0000) & 0b1111;
        }

        start();
        write(third + bright);
        stop();
    }
};

class knob {
   private:
    const int input;

    int count;

   public:
    knob(int input) : input(input) {
        pinMode(input, INPUT);

        count = 0;
    }

    int read(short inferior = 0, short superior = 100) {
        short value = analogRead(input);

        return map(value, 0, 1023, inferior, superior);
    }
};

class button {
   private:
    const int input;

   public:
    button(int input) : input(input) { pinMode(input, INPUT); }

    bool read() const {
        bool value = digitalRead(input);

        return value;
    }
};

class buzzer {
   private:
    const int input;

   public:
    buzzer(int input) : input(input) { pinMode(input, OUTPUT); }

    void start(int frequency, int duration = 0) {
        tone(input, frequency);

        if (duration) {
            delay(duration);
        }
    }

    void stop() { noTone(input); }
};
}  // namespace devices

#endif
