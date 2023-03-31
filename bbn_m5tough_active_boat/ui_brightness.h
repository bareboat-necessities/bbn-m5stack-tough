#ifndef UI_BRIGHTNESS_H
#define UI_BRIGHTNESS_H

#ifdef __cplusplus
extern "C" {
#endif

  int lcdVoltage = 2900;

  static void btnBacklight_event(lv_event_t *event) {
    // TODO:
    if (lcdVoltage > 2900) {
      lcdVoltage = 2500;
    } else {
      lcdVoltage = 3100;
    }
    M5.Axp.SetLDOVoltage(3, lcdVoltage);
  }


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
