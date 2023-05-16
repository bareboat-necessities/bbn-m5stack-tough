#ifndef UI_INIT_H
#define UI_INIT_H

#define LV_SYMBOL_DOUBLE_LEFT LV_SYMBOL_LEFT " " LV_SYMBOL_LEFT
#define LV_SYMBOL_DOUBLE_RIGHT LV_SYMBOL_RIGHT " " LV_SYMBOL_RIGHT
#define LV_SYMBOL_DEGREES "\xC2\xB0"

#ifdef __cplusplus
extern "C" {
#endif

  // init the tft espi
  static lv_disp_draw_buf_t draw_buf;
  static lv_disp_drv_t disp_drv;    // Descriptor of a display driver
  static lv_indev_drv_t indev_drv;  // Descriptor of a touch driver
  static unsigned long last_touched;

  M5Display *tft;

  void tft_lv_initialization() {
    M5.begin();

    preferences.begin("scr-cfg", false);
    bool rotate = preferences.getBool("ROTATE");
    if (rotate) {
      M5.Lcd.setRotation(3);
      //disp_drv.rotated = LV_DISP_ROT_180;
    }
    preferences.end();

    lv_init();

    static lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc((LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t)), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    static lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc((LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t)), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);

    // Initialize `disp_buf` display buffer with the buffer(s).
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, (LV_HOR_RES_MAX * LV_VER_RES_MAX));
    tft = &M5.Lcd;
  }

  // Display flushing
  void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft->startWrite();
    tft->setAddrWindow(area->x1, area->y1, w, h);
    tft->pushColors((uint16_t *)&color_p->full, w * h, true);
    tft->endWrite();

    lv_disp_flush_ready(disp);
  }

  void init_disp_driver() {
    lv_disp_drv_init(&disp_drv);  // Basic initialization

    disp_drv.flush_cb = my_disp_flush;  // Set your driver function
    disp_drv.draw_buf = &draw_buf;      // Assign the buffer to the display
    disp_drv.hor_res = LV_HOR_RES_MAX;  // Set the horizontal resolution of the display
    disp_drv.ver_res = LV_VER_RES_MAX;  // Set the vertical resolution of the display

    lv_disp_drv_register(&disp_drv);                   // Finally register the driver
    lv_disp_set_bg_color(NULL, lv_color_hex3(0x000));  // Set default background color to black
  }

  void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    TouchPoint_t pos = M5.Touch.getPressPoint();
    bool touched = (pos.x == -1) ? false : true;
    if (!touched) {
      data->state = LV_INDEV_STATE_RELEASED;
    } else {
      data->state = LV_INDEV_STATE_PRESSED;
      data->point.x = pos.x;
      data->point.y = pos.y;
      last_touched = millis();
    }
  }

  void init_touch_driver() {
    lv_disp_drv_register(&disp_drv);
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);  // register
    last_touched = millis();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
