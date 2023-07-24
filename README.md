# midi_to_freq: MIDI to Frequency Conversion Library
This repository contains a Python script named `midi_to_freq_code_gen.py` that generates C code for conversion of MIDI note data to frequencies and DDS (Direct Digital Synthesis) phase increments.

## How it Works
The library uses the formula f = 440 * 2^((n - 69) / 12) to calculate the frequency (f) of a MIDI note number (n). The reference pitch A440 (A4 = 440 Hz) is used as the base frequency.

Once the frequencies and phase increments are calculated, the library generates C code with Look-Up Tables (C arrays) for every note number, along with functions for accessing the values stored in the arrays. This approach enables efficient and quick retrieval of frequency and phase increment values for any MIDI note.

## Usage
To use this library, you need Python installed on your system. Follow these steps:
1. Clone or download this repository to your local machine.
2. Navigate to the repository directory using a terminal or command prompt.
3. Run the Python script to generate the C header and source files:
```bash
python midi_to_freq_code_gen.py [--phase_increment_width PHASE_WIDTH] [--dds_f_clk_mhz DDS_CLK_MHZ] [--output_directory OUTPUT_DIR]
```

* `--phase_increment_width`: (optional) Width of the phase increment in bits (default: 32).
* `--dds_f_clk_mhz`: (optional) Frequency of the DDS clock in MHz (default: 100).
* `--output_directory`: (optional) Directory where the generated files will be saved (default: "generated")

4. After running the script, you will find the generated C header file `midi_to_freq.h` and the C source file `midi_to_freq.c` in the specified output directory or the default `generated` directory.
5. Include `midi_to_freq.h` in your C project and link it with `midi_to_freq.c` during compilation to use the library's functionality.

Please refer to the generated C header file `midi_to_freq.h` for additional information on available functions and their usage.

## Example
Here is an example C file that uses the generated library to convert a MIDI note number to its corresponding frequency:

```c
#include <stdio.h>
#include <stdint.h>

#include "midi_to_freq.h"

int main() {
    uint8_t midi_note = 69; // MIDI note number for A4
    double frequency = midiToFreq(midi_note);
    uint32_t phase_increment = midiToPhaseIncr(midi_note);
    printf("MIDI Note: %d, Frequency: %.2f Hz, Phase Increment: %u\n", midi_note, frequency, phase_increment);

    return 0;
}
```
This program produces the following output:
```console
MIDI Note: 69, Frequency: 440.00 Hz, Phase Increment: 18898
```

The `example` directory contains the code above and the `CMakeList.txt` file that runs `midi_to_freq_code_gen.py` during the build process, so that there is no need to run it as an addtional step.

The generated code is tracked in this repository and stored in the `example/generated` directory, allowing you to review it before using the library.

## License
This library is licensed under the MIT License.
