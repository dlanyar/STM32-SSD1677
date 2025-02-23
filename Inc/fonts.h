/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

/* Max size of bitmap will based on a font24 (17x24) */
#define MAX_HEIGHT_FONT         24
#define MAX_WIDTH_FONT          17
#define OFFSET_BITMAP           54

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

struct sFONT {
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
};

extern struct sFONT Font72;
extern struct sFONT Font24;
extern struct sFONT Font20;
extern struct sFONT Font16;
extern struct sFONT Font12;
extern struct sFONT Font8;

#endif /* __FONTS_H */
