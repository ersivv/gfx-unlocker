/*
 * Copyright (c) 2013, Mateusz Tomaszkiewicz
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "unlocker.h"

int main(void)
{
  /*
   * This app is meant to be run in ChibiOS/RT in hardware, however unlocker
   * alone should also work in ChibiOS/RT simulator.
   */
  halInit();
  chSysInit();

  gfxInit();

  /* Touch sensor calibration if needed.*/
  ginputGetMouse(0);

  gdispSetOrientation(GDISP_ROTATE_90);

  /*
   * This programs new pattern.
   * Usually you run it inside "protected zone"; after the call
   * to the displayUnlocker() which should load secret pattern either
   * predefined in code or loaded from some kind of external memory.
   * Following sequence of calls is just an example.
   */
  uint8_t secret_sequence[UNLOCKER_ROWS * UNLOCKER_COLS];
  displayUnlockerSetup(&secret_sequence[0]);

  /*
   * Unlocker exits only if user draws proper pattern.
   * This should be run before anything else happens.
   */
  secret_sequence[0] = 1;
  secret_sequence[1] = 4;
  secret_sequence[2] = 8;
  secret_sequence[3] = 6;
  secret_sequence[4] = 2;
  secret_sequence[5] = 5;
  secret_sequence[6] = 7;
  secret_sequence[7] = 0;
  secret_sequence[8] = 0;
  displayUnlocker(&secret_sequence[0]);

  /* Continue with normal work.*/
  gdispClear(Black);
  font_t font;
  font = gdispOpenFont("UI2");
  gdispDrawString(30, 30, "Ready to go!", font, White);
  gdispCloseFont(font);

  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
