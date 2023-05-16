#ifndef HW_SLEEP_H
#define HW_SLEEP_H

#ifdef __cplusplus
extern "C" {
#endif

  void deep_sleep_with_touch_wakeup() {
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CST_INT, LOW);
    gpio_hold_en(GPIO_NUM_2);  // M5EPD_MAIN_PWR_PIN
    gpio_deep_sleep_hold_en();

    M5.Lcd.writecommand(ILI9341_DISPOFF);
    set_lcd_backlight_voltage(0);

    esp_deep_sleep_start();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
