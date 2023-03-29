#ifndef UI_THEME_H
#define UI_THEME_H

#ifdef __cplusplus
extern "C" {
#endif

  int theme = 1;

  void init_theme() {
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
  }

  void toggle_ui_theme() {
    theme = 1 - theme;
    init_theme();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
