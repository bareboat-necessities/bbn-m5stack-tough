#ifndef SHIP_DATA_UTIL_H
#define SHIP_DATA_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define TWO_MINUTES 120000
#define LONG_EXPIRE_TO 172800000

#define NM_TO_METERS 1852.0

  bool fresh(unsigned long age, unsigned long limit = 5000) {
    return millis() - age < limit && age != 0;
  }

  engine_t *lookup_engine(const char *engineID) {
    int last = -1;
    for (int i = 0; i < MAX_ENGINES; i++) {
      if (strncmp(shipDataModel.propulsion.engines[i].engine_label, engineID, MAX_ENGINE_LBL_LENGTH) == 0) {
        return &shipDataModel.propulsion.engines[i];
      }
      if (shipDataModel.propulsion.engines[i].engine_label[0] != 0) {
        last++;
      }
    }
    if ((last + 1) < MAX_ENGINES) {
      strncpy((char *)(shipDataModel.propulsion.engines[last + 1].engine_label), engineID, MAX_ENGINE_LBL_LENGTH);
      return &shipDataModel.propulsion.engines[last + 1];
    }
    return NULL;
  }


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
