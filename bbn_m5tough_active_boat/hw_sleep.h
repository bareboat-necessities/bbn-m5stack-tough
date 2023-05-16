#ifndef HW_SLEEP_H
#define HW_SLEEP_H

#ifdef __cplusplus
extern "C" {
#endif

  void deep_sleep_with_touch_wakeup() {
    /*
      You will need to call 
      WiFi.setSleep(WIFI_PS_NONE);
      somewhere after WiFi connection is established to avoid ghost interrupts
    */
    //WiFi.setSleep(false);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CST_INT, LOW);

    //  esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, LOW);  // TOUCH_INT
    gpio_hold_en(GPIO_NUM_2);  // M5EPD_MAIN_PWR_PIN
    gpio_deep_sleep_hold_en();
    M5.Lcd.writecommand(ILI9341_DISPOFF);
    M5.Lcd.setBrightness(0);

    esp_deep_sleep_start();
    //M5.Axp.DeepSleep(0);
  }

  void light_sleep_with_touch_wakeup() {
    /*
      You will need to call 
      WiFi.setSleep(WIFI_PS_NONE);
      somewhere after WiFi connection is established to avoid ghost interrupts
    */
    //WiFi.setSleep(false);
    //    esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, LOW);  // TOUCH_INT
    //    esp_sleep_enable_ext0_wakeup((gpio_num_t)CST_INT, LOW);
    gpio_wakeup_enable((gpio_num_t)CST_INT, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    M5.Lcd.writecommand(ILI9341_DISPOFF);
    M5.Lcd.setBrightness(0);

    esp_light_sleep_start();
    //M5.Axp.LightSleep(0);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
