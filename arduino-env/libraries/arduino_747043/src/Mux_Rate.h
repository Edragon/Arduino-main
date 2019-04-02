#ifndef MUX_RATE_H
#define MUX_RATE_H

typedef byte mux_rate;

class Mux_Rate {
    public:
        static const mux_rate HUNDRED = 0x10;
        static const mux_rate EIGHTY = 0x11;
        static const mux_rate SIXTY_FIVE = 0x12;
        static const mux_rate FORTY_EIGHT = 0x13;
        static const mux_rate FORTY = 0x14;
        static const mux_rate TWENTY_FOUR = 0x15;
        static const mux_rate EIGHTEEN = 0x16;
        static const mux_rate TEN = 0x17;
};

#endif
