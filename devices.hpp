#include "canvas.hpp"
#include "utils.hpp"

#ifndef DEVICES_HPP
#define DEVICES_HPP

namespace devices {
/**
 * @class screen
 * @brief Emulate a 8x8 led screen (MAX7219)
 *
 */
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
    /**
     * @brief Initialize screen
     *
     * @param input Input pin
     * @param clock Clock pin
     * @param load Load pin
     */
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

    /**
     * @brief Record data in an address in screen
     *
     * @param adress Address in screen
     * @param data Data to be recorded
     */
    void control(byte adress, byte data) const {
        digitalWrite(load, false);
        shiftOut(input, clock, MSBFIRST, adress);
        shiftOut(input, clock, MSBFIRST, data);
        digitalWrite(load, true);
    }

    /**
     * @brief Render a frame in screen
     *
     * @param pattern Frame to be rendered
     */
    void render(const canvas::frame &pattern) const {
        short index = short();

        for (const short &line : pattern) {
            control(++index, numerics::reverse(line));
        }
    }

    /**
     * @brief Render a frame in screen
     *
     * @param pattern Frame to be rendered
     */
    void render(const canvas::frame &&pattern) const { render(pattern); }

    /**
     * @brief Clear screen
     */
    void clear() const { render(canvas::frame()); }

    /**
     * @brief Change screen brightness
     *
     * @param bright Bright value
     */
    void brightness(byte bright) const {
        bright = map(bright, 0, 100, 1, 15);

        control(intensity, bright);
    }
};

/**
 * @class display
 * @brief Emulate a 4-digit 7-segment display (TM1637)
 *
 */
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
    /**
     * @brief Initialize display
     *
     * @param input Input pin
     * @param clock Clock pin
     * @param time Time pin
     */
    display(int input, int clock, int time) : input(input), clock(clock), time(time) {
        pinMode(input, INPUT);
        pinMode(clock, INPUT);

        digitalWrite(input, LOW);
        digitalWrite(clock, LOW);
    }

    /**
     * @brief Wait microseconds
     */
    void wait() const { delayMicroseconds(time); }

    /**
     * @brief Start to writing data in display
     */
    void start() const {
        pinMode(input, OUTPUT);
        wait();
    }

    /**
     * @brief Stop to writing data in display
     */
    void stop() const {
        pinMode(input, OUTPUT);
        wait();
        pinMode(clock, INPUT);
        wait();
        pinMode(input, INPUT);
        wait();
    }

    /**
     * @brief Write data in display
     *
     * @param data Data to be writted
     */
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

    /**
     * @brief Render layer in display
     *
     * @param segments Layer to be rendered
     */
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

    /**
     * @brief Render layer in display
     *
     * @param segments Layer to be rendered
     */
    void render(const canvas::layer &&segments) const { render(segments); }

    /**
     * @brief Clear display
     */
    void clear() const { render(canvas::layer()); }

    /**
     * @brief Enable separator
     *
     * @param value Enable value
     */
    void separator(bool value) { dots = value; }

    /**
     * @brief Enable and set brightness in display
     *
     * @param bright Bright
     * @param enable Enable
     */
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

/**
 * @class knob
 * @brief Knob
 *
 */
class knob {
   private:
    const int input;

    int count;

   public:
    /**
     * @brief Initialize knob
     *
     * @param input Input pin
     */
    knob(int input) : input(input) {
        pinMode(input, INPUT);

        count = 0;
    }

    /**
     * @brief Read knob position
     *
     * @param inferior Inferior range
     * @param superior Superior range
     * @return Position relative range
     */
    int read(short inferior = 0, short superior = 100) {
        short value = analogRead(input);

        return map(value, 0, 1023, inferior, superior);
    }
};

/**
 * @class button
 * @brief Push Button
 *
 */
class button {
   private:
    const int input;

   public:
    /**
     * @brief Initialize button
     *
     * @param input Input pin
     */
    button(int input) : input(input) { pinMode(input, INPUT); }

    /**
     * @brief Read state button
     *
     * @return Button state (pressed or unpressed)
     */
    bool read() const {
        bool value = digitalRead(input);

        return value;
    }
};

/**
 * @class buzzer
 * @brief Buzzer (it does not really used in project)
 *
 */
class buzzer {
   private:
    const int input;

   public:
    /**
     * @brief Initialize buzzer
     *
     * @param input Input pin
     */
    buzzer(int input) : input(input) { pinMode(input, OUTPUT); }

    /**
     * @brief Play a square wave in a frequency by a time
     *
     * @param frequency Frequency
     * @param duration Duration
     */
    void start(int frequency, int duration = 0) {
        tone(input, frequency);

        if (duration) {
            delay(duration);
        }
    }

    /**
     * @brief Stop playing
     */
    void stop() { noTone(input); }
};
}  // namespace devices

#endif
