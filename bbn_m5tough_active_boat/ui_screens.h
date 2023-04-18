#ifndef UI_SCREENS_H
#define UI_SCREENS_H

#ifdef __cplusplus
extern "C" {
#endif

  typedef void (*lv_update_screen_data_cb_t)();
  typedef void (*lv_init_screen_cb_t)(lv_obj_t* obj);

  static void noop_update_cb() {
  }

  static void noop_init_cb(lv_obj_t* parent) {
  }

  typedef struct _lv_updatable_screen_t {
    lv_obj_t* screen;
    lv_update_screen_data_cb_t update_cb = noop_update_cb;  // Default NoOp callback
    lv_init_screen_cb_t init_cb = noop_init_cb;
  } lv_updatable_screen_t;

  // triggers callback function set in update_cb
  void update_screen(lv_updatable_screen_t& screen) {
    (*screen.update_cb)();
  }

  // triggers callback function set in init_cb
  void init_screen(lv_updatable_screen_t& screen) {
    lv_obj_clean(screen.screen);
    (*screen.init_cb)(screen.screen);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
