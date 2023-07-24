"""
MIT License

Copyright (c) 2023 Pawel Kusinski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import argparse
import os


def calculate_note_name(note_number):
    notes = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
    note = notes[note_number % 12]
    octave = note_number // 12 - 1
    return f"{note}{octave}"


def calculate_frequency(note_number):
    return round(440 * (2 ** ((note_number - 69) / 12)), 2)


def calculate_phase_increment(frequency, phase_increment_width=32, dds_f_clk_mhz=100):
    return round((frequency * 2 ** (phase_increment_width)) / (dds_f_clk_mhz * 1000000))


def generate_h_file(filename, output_directory, copyright_string, phase_increment_width=32, dds_f_clk_mhz=100):
    note_enum_identifiers = [f"{calculate_note_name(note_number).replace('#', 'SHARP').replace('-', 'MINUS')} = {note_number}" for note_number in range(128)]
    note_enum_str = ",\n    ".join(note_enum_identifiers)
    file_template = '''\
{copyright}
#ifndef __MIDI_TO_FREQ__
#define __MIDI_TO_FREQ__

#include <stdint.h>

/* The enumeration provides named constants for each MIDI note, making it easy to refer to specific notes. */
typedef enum _MidiNote {{
    {notes},
    N_MIDI_NOTES
}} MidiNote;

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
 *   - phase_increment_width = {phase_increment_width} (width of the phase increment in bits)
 *   - dds_f_clk_mhz = {dds_f_clk_mhz} (frequency of the DDS clock in MHz)
 * The formula used to calculate the phase increment is:
 *   phase_increment = (f_desired * 2^phase_increment_width) / (dds_f_clk_mhz * 1000000)
 *
 * @param note_number The MIDI note number for which the phase increment is to be calculated.
 * @return The phase increment corresponding to the given MIDI note number.
 */
uint32_t midiToPhaseIncr(uint8_t note_number);

#endif
'''
    file_content = file_template.format(
        copyright=copyright_string.replace("{", "{{").replace("}", "}}"),
        notes=note_enum_str,
        phase_increment_width=phase_increment_width,
        dds_f_clk_mhz=dds_f_clk_mhz
    )
    
    # Ensure that the output directory exists, create it if needed
    os.makedirs(output_directory, exist_ok=True)

    with open(f"{output_directory}/{filename}", "w") as file:
        file.write(file_content)


def generate_c_file(filename, header_file_name, output_directory, copyright_string, phase_increment_width=32, dds_f_clk_mhz=100):
    freq_list = [calculate_frequency(note_number) for note_number in range(128)]
    note_names = [calculate_note_name(note_number) for note_number in range(128)]
    freq_with_notes_str = ",\n    ".join(f"{freq:.2f},  // {note_names[i]}" for i, freq in enumerate(freq_list))

    phase_incr_list = [calculate_phase_increment(freq, phase_increment_width, dds_f_clk_mhz) for freq in freq_list]
    phase_incr_with_notes_str = ",\n    ".join(f"{int(phase_incr)},  // {note_names[i]}" for i, phase_incr in enumerate(phase_incr_list))

    file_template = '''\
{copyright}
#include "{header}"

#include <stdint.h>
#include <assert.h>

static double frequencies[] = {{
    {freq}
}};

static uint32_t phase_increments[] = {{
    {phase_incr}
}};

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define PHASE_INCREMENT_WIDTH ({phase_increment_width})

static_assert(N_MIDI_NOTES == ARRAY_LEN(frequencies), "Invalid length of frequencies array");
static_assert(N_MIDI_NOTES == ARRAY_LEN(phase_increments), "Invalid length of phase_increments array");
static_assert(PHASE_INCREMENT_WIDTH <= 8 * sizeof((phase_increments)[0]), "PHASE_INCREMENT_WIDTH of {phase_increment_width} is not supported");

double midiToFreq(uint8_t note_number) {{
    if (note_number >= ARRAY_LEN(frequencies)) {{
        return 0;
    }}

    return frequencies[note_number];
}}

uint32_t midiToPhaseIncr(uint8_t note_number) {{
    if (note_number >= ARRAY_LEN(phase_increments)) {{
        return 0;
    }}

    return phase_increments[note_number];
}}
'''
    file_content = file_template.format(
        copyright=copyright_string.replace("{", "{{").replace("}", "}}"),
        header=header_file_name,
        freq=freq_with_notes_str,
        phase_incr=phase_incr_with_notes_str,
        phase_increment_width=phase_increment_width,
        dds_f_clk_mhz=dds_f_clk_mhz
    )
    
    # Ensure that the output directory exists, create it if needed
    os.makedirs(output_directory, exist_ok=True)

    with open(f"{output_directory}/{filename}", "w") as file:
        file.write(file_content)



if __name__ == "__main__":
    copyright_text = """
/*====================================================================================================================*\\
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
"""
    header_file_name = "midi_to_freq.h"
    c_file_name = "midi_to_freq.c"

    parser = argparse.ArgumentParser(description="Generate MIDI to Frequency C code.")
    parser.add_argument("--phase_increment_width", type=int, default=32, help="Width of the phase increment.")
    parser.add_argument("--dds_f_clk_mhz", type=int, default=100, help="Frequency of the DDS clock in MHz.")
    parser.add_argument("--output_directory", default="generated", help="Output directory for generated files (default: generated)")
    args = parser.parse_args()

    generate_h_file(
        header_file_name,
        args.output_directory,
        copyright_text,
        phase_increment_width=args.phase_increment_width,
        dds_f_clk_mhz=args.dds_f_clk_mhz)

    generate_c_file(
        c_file_name,
        header_file_name,
        args.output_directory,
        copyright_text,
        phase_increment_width=args.phase_increment_width,
        dds_f_clk_mhz=args.dds_f_clk_mhz
    )
