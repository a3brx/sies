#include <lcom/lcf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

unsigned sys_inb_counter = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  // Stores the lsb of the value on the given address
  // val    - input 2 byte integer
  // lsb    - address of memory location to be updated with val's LSB
  // Return - 0 upon success and non-zero otherwise
  *lsb = val;                                                         // Just stores the value on the address
  return 0;                                                           // Successful operation
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  // Stores the msb of the value on the given address
  // val    - input 2 byte integer
  // msb    - address of memory location to be updated with val's MSB
  // Return - 0 upon success and non-zero otherwise
  val >>= 8;                                                          // Changes value to represent just the msb
  *msb = val;                                                         // Stores the value on the address
  return 0;                                                           // Successful operation
}

int (util_sys_inb)(int port, uint8_t *value) {
  // Loads the value from the port to the given address
  // port   -	the input port that is to be read
  // value  -	address of 8-bit variable to be update with the value read
  // Return - 0 upon success and non-zero otherwise
  u32_t val;                                                          // Creates a new variable to load the value from the port
  int res = sys_inb(port, &val);                                      // Loads the value from the port to the variable
  sys_inb_counter++;                                                  // Increments sys_inb_counter
  *value = (uint8_t) val;                                             // Stores the value on the given address
  return res;                                                         // Success of the sys_inb function
}

int (util_sys_in2b)(int port, uint16_t *value) {
  // Loads the value from the port to the given address
  // port   -	the input port that is to be read
  // value  -	address of 8-bit variable to be update with the value read
  // Return - 0 upon success and non-zero otherwise
  u32_t val;                                                          // Creates a new variable to load the value from the port
  int res = sys_inb(port, &val);                                      // Loads the value from the port to the variable
  *value = (uint16_t) val;                                            // Stores the value on the given address
  return res;                                                         // Success of the sys_inb function
}

char *util_itoa(int num) {
    if (num == 0) {
      return "00";
    }
    int aux = num, size = 0;
    char car[2];
    char *str_aux;
    car[1] = '\0';
    char *str;
    if (num >= 10) {
      str = (char *)malloc(1);
      strcpy(str, "\0");
    }
    else {
      str = (char *)malloc(2);
      strcpy(str, "0\0");
    }
    while (aux != 0) {
        car[0] = '0' + (aux % 10);
        str_aux = str;
        realloc(str, strlen(str_aux) + 1);
        strcat(str, car);
        aux /= 10;
        size++;
    }
    char temp;
    for (int i = 0; i < size/2; i++) {
        temp = str[i];
        str[i] = str[size-1-i];
        str[size-1-i] = temp;
    }
    return str;
}

void wait(int micros) {
    tickdelay(micros_to_ticks(micros));
}

uint8_t bcd_to_decimal(uint8_t x) {
    return x - 6 * (x >> 4);
}
