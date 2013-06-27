/*
 * Copyright (c) 2013, Mateusz Tomaszkiewicz <silentdemon@gmail.com>
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

static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {
  uint8_t s = 0;
  coord_t center_x = gdispGetWidth() / 2;
  coord_t center_y = gdispGetHeight() / 2;

  while (TRUE) {
    if (s == 0) {
      gdispFillArea(center_x - 10, center_y - 10, 20, 20, Red);
      s = 1;
    } else {
      gdispFillArea(center_x - 10, center_y - 10, 20, 20, Green);
      s = 0;
    }
    chThdSleepMilliseconds(500);
  }
}

int main(void)
{
  /*
   * This app is meant to be run in ChibiOS/RT in hardware, however unlocker
   * alone also works in ChibiOS/RT simulator.
   */
  halInit();
  chSysInit();

  gfxInit();

  /* Initialize mouse and calibrate it if needed.*/
  ginputGetMouse(0);

  gdispSetOrientation(GDISP_ROTATE_90);

  /*
   * This programs new pattern.
   * Usually you run it inside "protected zone"; after the call
   * to the displayUnlocker() which should load secret pattern either
   * predefined in code or loaded from some kind of external memory.
   * Following sequence of calls is just an example.
   */
  uint8_t secret_sequence[UNLOCKER_COLS * UNLOCKER_ROWS] = {1,4,8,6,2,5,7,0,0};
  displayUnlockerSetup(&secret_sequence[0]);

  /*
   * Unlocker exits only if user draws proper pattern.
   * This should be run before anything else happens.
   * Consider use of chSysLock() before and chSysUnlock() after.
   */
  displayUnlocker(&secret_sequence[0]);

  /* Continue with normal work.*/
  gdispClear(Black);
  font_t font;
  font = gdispOpenFont("UI2");
  gdispDrawString(30, 30, "Ready to go!", font, White);
  gdispCloseFont(font);

  chThdCreateStatic(waThread1, sizeof(waThread1),
                    NORMALPRIO, Thread1, NULL);

  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
