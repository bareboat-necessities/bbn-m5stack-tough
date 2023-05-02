#ifndef UI_PLAYER_CONTROL_H
#define UI_PLAYER_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t playerScreen;

  static lv_obj_t *player_display;

  static const char *player_btnm_map[] = {
    LV_SYMBOL_SHUFFLE,
    LV_SYMBOL_PREV,
    LV_SYMBOL_PLAY,
    LV_SYMBOL_NEXT,
    LV_SYMBOL_LOOP,
    "",
  };

  /**
   * A player display 
   */
  static void lv_player_display(lv_obj_t *parent) {
    player_display = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(player_display, player_btnm_map);
    lv_obj_set_size(player_display, LV_HOR_RES_MAX, 78);
    lv_obj_align(player_display, LV_ALIGN_CENTER, 0, 75);
    //lv_obj_add_event(player_display, event_handler, LV_EVENT_ALL, NULL);
  }

  static void player_update_cb() {
  }

  void init_playerScreen() {
    playerScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    playerScreen.init_cb = lv_player_display;
    playerScreen.update_cb = player_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
