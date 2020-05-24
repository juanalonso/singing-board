/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "synth_pulsetrain.h"
#include "arm_math.h"
#include "utility/dspinst.h"


/******************************************************************/
// PAH 140415 - change sin to use Paul's interpolation which is much
//				faster than arm's sin function
// PAH 140316 - fix calculation of sample (amplitude error)
// PAH 140314 - change t_hi from int to float


void AudioSynthPulsetrain::update(void)
{
  audio_block_t *block;
  uint32_t i;
  int32_t out;

  if (magnitude == 0) {
    return;
  }

  block = allocate();
  if (!block) {
    return;
  }

  for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
    elapsed += 65536;
    if (elapsed < duration) {
      out = 0;
    } else {
      elapsed -= duration;
      // elapsed must be 0 to 65535
      // magnitude must be -32767 to +32767
      out = 1000; //magnitude - ((magnitude * elapsed) >> 15);
      //magnitude = -magnitude;
    }
    block->data[i] = out;
  }

  transmit(block);
  release(block);
}




