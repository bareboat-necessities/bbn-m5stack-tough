#ifndef NET_HTTP_H
#define NET_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

  String httpGETRequest(const char* url) {
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();

    String payload = "";
    if (httpResponseCode > 0) {
      payload = http.getString();
    }
    http.end();
    return payload;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
