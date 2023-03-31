#ifndef MEM_CPU_NET_STAT_H
#define MEM_CPU_NET_STAT_H

#ifdef __cplusplus
extern "C" {
#endif

  void printDeviceStats() {
    M5.Lcd.printf("FreeHeap: %d bytes\n", ESP.getFreeHeap());
    M5.Lcd.printf("MinFreeHeap: %d bytes\n", ESP.getMinFreeHeap());
    M5.Lcd.printf("FreePsram: %d bytes\n", ESP.getFreePsram());
    M5.Lcd.printf("MinFreePsram: %d bytes\n", ESP.getMinFreePsram());
    M5.Lcd.printf("FreeSketchSpace: %d bytes\n", ESP.getFreeSketchSpace());
    M5.Lcd.printf("HeapSize: %d bytes\n", ESP.getHeapSize());

    M5.Lcd.printf("FlashChipSize: %d bytes\n", ESP.getFlashChipSize());
    M5.Lcd.printf("MaxAllocHeap: %d bytes\n", ESP.getMaxAllocHeap());
    M5.Lcd.printf("MaxAllocPsram: %d bytes\n", ESP.getMaxAllocPsram());
    M5.Lcd.printf("ChipCores: %d\n", ESP.getChipCores());
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
