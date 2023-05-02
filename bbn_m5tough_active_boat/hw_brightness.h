#ifndef HW_BRIGHTNESS_H
#define HW_BRIGHTNESS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LCD_V 3300
#define MIN_LCD_V 2500

  static void set_lcd_backlight_voltage(int pct) {
    if (pct >= 0 && pct <= 100) {
      int lcdVoltage = MIN_LCD_V + (MAX_LCD_V - MIN_LCD_V) * pct / 100;
      M5.Axp.SetLDOVoltage(3, lcdVoltage);  // MIN_LCD_V to MAX_LCD_V
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
