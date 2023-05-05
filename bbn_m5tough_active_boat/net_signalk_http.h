#ifndef NET_SIGNALK_HTTP_H
#define NET_SIGNALK_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

  void getVesselInfo() {
    String api = "http://192.168.1.34:3000/signalk/v1/api/"; // TODO:
    String resp = httpGETRequest((api + "vessels/self/design/beam/value/").c_str());
    if (resp.length() > 0) {
      shipDataModel.design.beam.m = resp.toFloat();
      shipDataModel.design.beam.age = millis();
    }
    resp = httpGETRequest((api + "vessels/self/name/").c_str());
    if (resp.length() > 0) {
      strncpy(shipDataModel.vessel.name, resp.c_str(), sizeof(shipDataModel.vessel.name) - 1);
    }
    resp = httpGETRequest((api + "vessels/self/mmsi/").c_str());
    if (resp.length() > 0) {
      strncpy(shipDataModel.vessel.mmsi, resp.c_str(), sizeof(shipDataModel.vessel.mmsi) - 1);
    }
  }


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
