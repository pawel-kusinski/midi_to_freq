
/*====================================================================================================================*\
 * MIT License
 *
 * Copyright (c) 2023 Pawel Kusinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
\*====================================================================================================================*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                           _     _ _  _          __                                                  *
 *                                          (_)   | (_)| |        / _|                                                 *
 *                                 _ __ ___  _  __| |_ | |_ ___  | |_ _ __ ___  __ _                                   *
 *                                | '_ ` _ \| |/ _` | || __/ _ \ |  _| '__/ _ \/ _` |                                  *
 *                                | | | | | | | (_| | || || (_) || | | | |  __| (_| |                                  *
 *                                |_| |_| |_|_|\__,_|_| \__\___/ |_| |_|  \___|\__, |                                  *
 *                                                  ______   ______               | |                                  *
 *                                                 |______| |______|              |_|                                  *
 *                                                                                                                     *  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 * This file has been generated. Do not edit.
 * midi_to_freq - a library that converts MIDI note data to frequency and DDS Phase Accumulator phase increment.
 * 
 */

#ifndef __MIDI_TO_FREQ__
#define __MIDI_TO_FREQ__

#include <stdint.h>

/* The enumeration provides named constants for each MIDI note, making it easy to refer to specific notes. */
typedef enum _MidiNote {
    CMINUS1 = 0,
    CSHARPMINUS1 = 1,
    DMINUS1 = 2,
    DSHARPMINUS1 = 3,
    EMINUS1 = 4,
    FMINUS1 = 5,
    FSHARPMINUS1 = 6,
    GMINUS1 = 7,
    GSHARPMINUS1 = 8,
    AMINUS1 = 9,
    ASHARPMINUS1 = 10,
    BMINUS1 = 11,
    C0 = 12,
    CSHARP0 = 13,
    D0 = 14,
    DSHARP0 = 15,
    E0 = 16,
    F0 = 17,
    FSHARP0 = 18,
    G0 = 19,
    GSHARP0 = 20,
    A0 = 21,
    ASHARP0 = 22,
    B0 = 23,
    C1 = 24,
    CSHARP1 = 25,
    D1 = 26,
    DSHARP1 = 27,
    E1 = 28,
    F1 = 29,
    FSHARP1 = 30,
    G1 = 31,
    GSHARP1 = 32,
    A1 = 33,
    ASHARP1 = 34,
    B1 = 35,
    C2 = 36,
    CSHARP2 = 37,
    D2 = 38,
    DSHARP2 = 39,
    E2 = 40,
    F2 = 41,
    FSHARP2 = 42,
    G2 = 43,
    GSHARP2 = 44,
    A2 = 45,
    ASHARP2 = 46,
    B2 = 47,
    C3 = 48,
    CSHARP3 = 49,
    D3 = 50,
    DSHARP3 = 51,
    E3 = 52,
    F3 = 53,
    FSHARP3 = 54,
    G3 = 55,
    GSHARP3 = 56,
    A3 = 57,
    ASHARP3 = 58,
    B3 = 59,
    C4 = 60,
    CSHARP4 = 61,
    D4 = 62,
    DSHARP4 = 63,
    E4 = 64,
    F4 = 65,
    FSHARP4 = 66,
    G4 = 67,
    GSHARP4 = 68,
    A4 = 69,
    ASHARP4 = 70,
    B4 = 71,
    C5 = 72,
    CSHARP5 = 73,
    D5 = 74,
    DSHARP5 = 75,
    E5 = 76,
    F5 = 77,
    FSHARP5 = 78,
    G5 = 79,
    GSHARP5 = 80,
    A5 = 81,
    ASHARP5 = 82,
    B5 = 83,
    C6 = 84,
    CSHARP6 = 85,
    D6 = 86,
    DSHARP6 = 87,
    E6 = 88,
    F6 = 89,
    FSHARP6 = 90,
    G6 = 91,
    GSHARP6 = 92,
    A6 = 93,
    ASHARP6 = 94,
    B6 = 95,
    C7 = 96,
    CSHARP7 = 97,
    D7 = 98,
    DSHARP7 = 99,
    E7 = 100,
    F7 = 101,
    FSHARP7 = 102,
    G7 = 103,
    GSHARP7 = 104,
    A7 = 105,
    ASHARP7 = 106,
    B7 = 107,
    C8 = 108,
    CSHARP8 = 109,
    D8 = 110,
    DSHARP8 = 111,
    E8 = 112,
    F8 = 113,
    FSHARP8 = 114,
    G8 = 115,
    GSHARP8 = 116,
    A8 = 117,
    ASHARP8 = 118,
    B8 = 119,
    C9 = 120,
    CSHARP9 = 121,
    D9 = 122,
    DSHARP9 = 123,
    E9 = 124,
    F9 = 125,
    FSHARP9 = 126,
    G9 = 127,
    N_MIDI_NOTES
} MidiNote;

/* Gets the frequency of the corresponding MIDI note.
 * The frequencies are calculated based on the A440 pitch standard (A4 = 440 Hz).
 *
 * @param note_number The MIDI note number for which the frequency is to be calculated. The MidiNote enumeration
 *                    can be used as a reference when a specific note number needs to be determined.
 * @return The frequency in Hertz (Hz) corresponding to the given MIDI note number.
 */
double midiToFreq(uint8_t note_number);

/* This function returns the phase increment for a given MIDI note number, which is useful for Direct Digital
 * Synthesis (DDS) systems. Direct Digital Synthesis (DDS) is a technique used to generate waveforms digitally
 * by incrementing a phase accumulator.
 * The phase increment is determined based on the desired frequency (f_desired) and the provided parameters:
 *   - phase_increment_width = 32 (width of the phase increment in bits)
 *   - dds_f_clk_mhz = 100 (frequency of the DDS clock in MHz)
 * The formula used to calculate the phase increment is:
 *   phase_increment = (f_desired * 2^phase_increment_width) / (dds_f_clk_mhz * 1000000)
 *
 * @param note_number The MIDI note number for which the phase increment is to be calculated.
 * @return The phase increment corresponding to the given MIDI note number.
 */
uint32_t midiToPhaseIncr(uint8_t note_number);

#endif
