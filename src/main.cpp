#include <stdio.h>
#include <robotcontrol.h>

// Headers for nonblocking stdin + console
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int main() {
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);  // set stdin (file descriptor 0) to nonblocking mode

  // Initialize the Pause (PAU) button
  rc_button_init(RC_BTN_PIN_PAUSE, RC_BTN_POLARITY_NORM_HIGH,
      RC_BTN_DEBOUNCE_DEFAULT_US);

  // A quick printf as a sanity check
  // Note, the \n is necessary since the buffer flushes on a line break
  printf("Hello World!\n");

  // A quick 0.5s LED pulse as an indicator the LED works
  rc_led_set(RC_LED_GREEN, 1);
  rc_usleep(500*1000);
  rc_led_set(RC_LED_GREEN, 0);

  uint32_t i = 0;
  while (1) {
    char buf[128];  // buffer to hold bytes read
    int numRead = read(0, buf, 128);  // returns number of bytes read, up to 128
    while (numRead > 0 && (buf[numRead - 1] == '\n' || buf[numRead - 1] == '\r')) {
      numRead--;  // strip trailing newline characters
    }
    if (numRead > 0) {  // if read returned something that was "nonempty"
      buf[numRead] = '\0';  // read() doesn't attach a null terminator, which makes strcmp and print happier
      if (strcmp(buf, "quit") == 0) {
        break;  // break out of the main loop, so the cleanup code can run
      } else {
        printf("unknown command '%s'\n", buf);
      }
    }
    
    
    // This simple code sets the LED equal to the state of the Pause (PAU) button.
    // Note that rc_button_get_state returns 1 when pressed.
    rc_led_set(RC_LED_RED, 1);
    rc_usleep(500*1000);
    rc_led_set(RC_LED_RED, 0);

    printf("Iteration %i\n", i);
    rc_usleep(500*1000);  // Limit console spam
    i++;
  }

  rc_led_cleanup();
  rc_button_cleanup();

  return 0;
}
