#ifndef UI_SCREENS_H
#define UI_SCREENS_H

#ifdef __cplusplus
extern "C" {
#endif

  typedef void (*lv_update_screen_data_cb_t)();

  static void noop_update_cb() {
  }

  typedef struct _lv_updatable_screen_t {
    lv_obj_t* screen = lv_obj_create(NULL); 
    lv_update_screen_data_cb_t update_cb = noop_update_cb; // Default NoOp callback
  } lv_updatable_screen_t;

  int page = 0;

  typedef lv_updatable_screen_t screens_arr_t[];

  // triggers callback function set in update_cb 
  void update_screen(lv_updatable_screen_t& screen) {
    (*screen.update_cb)();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
