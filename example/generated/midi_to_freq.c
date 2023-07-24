
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

#include "midi_to_freq.h"

#include <stdint.h>
#include <assert.h>

static double frequencies[] = {
    8.18,  // C-1,
    8.66,  // C#-1,
    9.18,  // D-1,
    9.72,  // D#-1,
    10.30,  // E-1,
    10.91,  // F-1,
    11.56,  // F#-1,
    12.25,  // G-1,
    12.98,  // G#-1,
    13.75,  // A-1,
    14.57,  // A#-1,
    15.43,  // B-1,
    16.35,  // C0,
    17.32,  // C#0,
    18.35,  // D0,
    19.45,  // D#0,
    20.60,  // E0,
    21.83,  // F0,
    23.12,  // F#0,
    24.50,  // G0,
    25.96,  // G#0,
    27.50,  // A0,
    29.14,  // A#0,
    30.87,  // B0,
    32.70,  // C1,
    34.65,  // C#1,
    36.71,  // D1,
    38.89,  // D#1,
    41.20,  // E1,
    43.65,  // F1,
    46.25,  // F#1,
    49.00,  // G1,
    51.91,  // G#1,
    55.00,  // A1,
    58.27,  // A#1,
    61.74,  // B1,
    65.41,  // C2,
    69.30,  // C#2,
    73.42,  // D2,
    77.78,  // D#2,
    82.41,  // E2,
    87.31,  // F2,
    92.50,  // F#2,
    98.00,  // G2,
    103.83,  // G#2,
    110.00,  // A2,
    116.54,  // A#2,
    123.47,  // B2,
    130.81,  // C3,
    138.59,  // C#3,
    146.83,  // D3,
    155.56,  // D#3,
    164.81,  // E3,
    174.61,  // F3,
    185.00,  // F#3,
    196.00,  // G3,
    207.65,  // G#3,
    220.00,  // A3,
    233.08,  // A#3,
    246.94,  // B3,
    261.63,  // C4,
    277.18,  // C#4,
    293.66,  // D4,
    311.13,  // D#4,
    329.63,  // E4,
    349.23,  // F4,
    369.99,  // F#4,
    392.00,  // G4,
    415.30,  // G#4,
    440.00,  // A4,
    466.16,  // A#4,
    493.88,  // B4,
    523.25,  // C5,
    554.37,  // C#5,
    587.33,  // D5,
    622.25,  // D#5,
    659.26,  // E5,
    698.46,  // F5,
    739.99,  // F#5,
    783.99,  // G5,
    830.61,  // G#5,
    880.00,  // A5,
    932.33,  // A#5,
    987.77,  // B5,
    1046.50,  // C6,
    1108.73,  // C#6,
    1174.66,  // D6,
    1244.51,  // D#6,
    1318.51,  // E6,
    1396.91,  // F6,
    1479.98,  // F#6,
    1567.98,  // G6,
    1661.22,  // G#6,
    1760.00,  // A6,
    1864.66,  // A#6,
    1975.53,  // B6,
    2093.00,  // C7,
    2217.46,  // C#7,
    2349.32,  // D7,
    2489.02,  // D#7,
    2637.02,  // E7,
    2793.83,  // F7,
    2959.96,  // F#7,
    3135.96,  // G7,
    3322.44,  // G#7,
    3520.00,  // A7,
    3729.31,  // A#7,
    3951.07,  // B7,
    4186.01,  // C8,
    4434.92,  // C#8,
    4698.64,  // D8,
    4978.03,  // D#8,
    5274.04,  // E8,
    5587.65,  // F8,
    5919.91,  // F#8,
    6271.93,  // G8,
    6644.88,  // G#8,
    7040.00,  // A8,
    7458.62,  // A#8,
    7902.13,  // B8,
    8372.02,  // C9,
    8869.84,  // C#9,
    9397.27,  // D9,
    9956.06,  // D#9,
    10548.08,  // E9,
    11175.30,  // F9,
    11839.82,  // F#9,
    12543.85,  // G9
};

static uint32_t phase_increments[] = {
    351,  // C-1,
    372,  // C#-1,
    394,  // D-1,
    417,  // D#-1,
    442,  // E-1,
    469,  // F-1,
    496,  // F#-1,
    526,  // G-1,
    557,  // G#-1,
    591,  // A-1,
    626,  // A#-1,
    663,  // B-1,
    702,  // C0,
    744,  // C#0,
    788,  // D0,
    835,  // D#0,
    885,  // E0,
    938,  // F0,
    993,  // F#0,
    1052,  // G0,
    1115,  // G#0,
    1181,  // A0,
    1252,  // A#0,
    1326,  // B0,
    1404,  // C1,
    1488,  // C#1,
    1577,  // D1,
    1670,  // D#1,
    1770,  // E1,
    1875,  // F1,
    1986,  // F#1,
    2105,  // G1,
    2230,  // G#1,
    2362,  // A1,
    2503,  // A#1,
    2652,  // B1,
    2809,  // C2,
    2976,  // C#2,
    3153,  // D2,
    3341,  // D#2,
    3539,  // E2,
    3750,  // F2,
    3973,  // F#2,
    4209,  // G2,
    4459,  // G#2,
    4724,  // A2,
    5005,  // A#2,
    5303,  // B2,
    5618,  // C3,
    5952,  // C#3,
    6306,  // D3,
    6681,  // D#3,
    7079,  // E3,
    7499,  // F3,
    7946,  // F#3,
    8418,  // G3,
    8918,  // G#3,
    9449,  // A3,
    10011,  // A#3,
    10606,  // B3,
    11237,  // C4,
    11905,  // C#4,
    12613,  // D4,
    13363,  // D#4,
    14158,  // E4,
    14999,  // F4,
    15891,  // F#4,
    16836,  // G4,
    17837,  // G#4,
    18898,  // A4,
    20021,  // A#4,
    21212,  // B4,
    22473,  // C5,
    23810,  // C#5,
    25226,  // D5,
    26725,  // D#5,
    28315,  // E5,
    29999,  // F5,
    31782,  // F#5,
    33672,  // G5,
    35674,  // G#5,
    37796,  // A5,
    40043,  // A#5,
    42424,  // B5,
    44947,  // C6,
    47620,  // C#6,
    50451,  // D6,
    53451,  // D#6,
    56630,  // E6,
    59997,  // F6,
    63565,  // F#6,
    67344,  // G6,
    71349,  // G#6,
    75591,  // A6,
    80087,  // A#6,
    84848,  // B6,
    89894,  // C7,
    95239,  // C#7,
    100903,  // D7,
    106903,  // D#7,
    113259,  // E7,
    119994,  // F7,
    127129,  // F#7,
    134688,  // G7,
    142698,  // G#7,
    151183,  // A7,
    160173,  // A#7,
    169697,  // B7,
    179788,  // C8,
    190478,  // C#8,
    201805,  // D8,
    213805,  // D#8,
    226518,  // E8,
    239988,  // F8,
    254258,  // F#8,
    269377,  // G8,
    285395,  // G#8,
    302366,  // A8,
    320345,  // A#8,
    339394,  // B8,
    359576,  // C9,
    380957,  // C#9,
    403610,  // D9,
    427610,  // D#9,
    453037,  // E9,
    479975,  // F9,
    508516,  // F#9,
    538754,  // G9
};

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define PHASE_INCREMENT_WIDTH (32)

static_assert(N_MIDI_NOTES == ARRAY_LEN(frequencies), "Invalid length of frequencies array");
static_assert(N_MIDI_NOTES == ARRAY_LEN(phase_increments), "Invalid length of phase_increments array");
static_assert(PHASE_INCREMENT_WIDTH <= 8 * sizeof((phase_increments)[0]), "PHASE_INCREMENT_WIDTH of 32 is not supported");

double midiToFreq(uint8_t note_number) {
    if (note_number >= ARRAY_LEN(frequencies)) {
        return 0;
    }

    return frequencies[note_number];
}

uint32_t midiToPhaseIncr(uint8_t note_number) {
    if (note_number >= ARRAY_LEN(phase_increments)) {
        return 0;
    }

    return phase_increments[note_number];
}
