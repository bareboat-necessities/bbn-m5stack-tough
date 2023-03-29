#ifndef UI_GESTURES_H
#define UI_GESTURES_H

#ifdef __cplusplus
extern "C" {
#endif

  Gesture swipeDown("swipe down", 80, DIR_DOWN, 40);
  Gesture swipeUp("swipe up", 80, DIR_UP, 40);

  Gesture swipeLeft("swipe left", 80, DIR_LEFT, 40);
  Gesture swipeRight("swipe right", 80, DIR_RIGHT, 40);

  bool swipe_vert_detected() {
    return swipeDown.wasDetected() || swipeUp.wasDetected();
  }

  bool swipe_left_detected() {
    return swipeRight.wasDetected();
  }

  bool swipe_right_detected() {
    return swipeRight.wasDetected();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
