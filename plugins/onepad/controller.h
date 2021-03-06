/*  OnePAD - author: arcum42(@gmail.com)
 *  Copyright (C) 2009
 *
 *  Based on ZeroPAD, author zerofrog@gmail.com
 *  Copyright (C) 2006-2007
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#pragma once
#include <string.h> // for memset
#define MAX_KEYS 24

extern void set_keyboard_key(int pad, int keysym, int index);
extern int get_keyboard_key(int pad, int keysym);
extern bool IsAnalogKey(int index);

class PADconf
{
    u32 ff_intensity;
    u32 sensibility;

public:
    union
    {
        struct
        {
            u16 forcefeedback : 1;
            u16 reverse_lx : 1;
            u16 reverse_ly : 1;
            u16 reverse_rx : 1;
            u16 reverse_ry : 1;
            u16 mouse_l : 1;
            u16 mouse_r : 1;
            u16 _free : 9;             // The 9 remaining bits are unused, do what you wish with them ;)
        } pad_options[GAMEPAD_NUMBER]; // One for each pads
        u32 packed_options;            // Only first 8 bits of each 16 bits series are really used, rest is padding
    };

    u32 log;
    std::map<u32, u32> keysym_map[GAMEPAD_NUMBER];
    std::array<size_t, GAMEPAD_NUMBER> unique_id;
    std::vector<std::string> sdl2_mapping;

    PADconf() { init(); }

    void init()
    {
        log = packed_options = 0;
        ff_intensity = 0x7FFF; // set it at max value by default
        sensibility = 500;
        for (int pad = 0; pad < GAMEPAD_NUMBER; pad++) {
            keysym_map[pad].clear();
        }
        unique_id.fill(0);
        sdl2_mapping.clear();
    }

    void set_joy_uid(u32 pad, size_t uid)
    {
        if (pad < GAMEPAD_NUMBER)
            unique_id[pad] = uid;
    }

    size_t get_joy_uid(u32 pad)
    {
        if (pad < GAMEPAD_NUMBER)
            return unique_id[pad];
        else
            return 0;
    }

    /**
	 * Return (a copy of) private memner ff_instensity
	 **/
    u32 get_ff_intensity()
    {
        return ff_intensity;
    }

    /**
	 * Set intensity while checking that the new value is within
	 * valid range, more than 0x7FFF will cause pad not to rumble(and less than 0 is obviously bad)
	 **/
    void set_ff_intensity(u32 new_intensity)
    {
        if (new_intensity <= 0x7FFF) {
            ff_intensity = new_intensity;
        }
    }

    /**
	 * Set sensibility value, sensibility is not yet implemented(and will probably be after evdev)
	 * However, there will be an upper range too, less than 0 is an obvious wrong
	 * Anyway, we are doing object oriented code, members are definitely not supposed to be public
	 **/
    void set_sensibility(u32 new_sensibility)
    {
        if (sensibility > 0) {
            sensibility = new_sensibility;
        }
    }

    u32 get_sensibility()
    {
        return sensibility;
    }
};
extern PADconf g_conf;
