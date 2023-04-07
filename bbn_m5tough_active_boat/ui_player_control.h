#ifndef UI_PLAYER_CONTROL_H
#define UI_PLAYER_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t playerScreen;

  static lv_obj_t *player_display;

static const char *btnm_map[] = {
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
void lv_player_display(lv_obj_t *parent) {
  lv_obj_t *player_display = lv_btnmatrix_create(parent);
  lv_btnmatrix_set_map(player_display, btnm_map);
   lv_obj_set_size(player_display, 320, 78);
  //lv_btnmatrix_set_btn_width(player_display, 15, 2);
  //lv_btnmatrix_set_btn_height(player_display, 15, 2);
  lv_obj_align(player_display, LV_ALIGN_CENTER, 0, 75);
  //lv_obj_add_event(player_display, event_handler, LV_EVENT_ALL, NULL);
}


  
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
