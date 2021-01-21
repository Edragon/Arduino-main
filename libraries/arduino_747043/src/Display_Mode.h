#ifndef DISPLAY_MODE_H
#define DISPLAY_MODE_H

typedef byte display_mode;
        
class Display_Mode {
    public:
        static const display_mode BLANK = 0x8;
        static const display_mode NORMAL = 0xc;
        static const display_mode ALL_SEGMENTS_ON = 0x9;
        static const display_mode INVERSE_VIDEO = 0xd;
};

#endif
