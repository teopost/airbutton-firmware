#include <Arduino.h>
#include <ESP8266httpUpdate.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>

#include "OtaUpdate.h"

OtaUpdate::OtaUpdate(String firmwareUrl, int localRelease)
{
  _firmwareUrl = firmwareUrl;
  _localRelease = localRelease;
  _releaseUrl = firmwareUrl + ".txt";
}

void OtaUpdate::Reboot()
{
  ESP.restart();
}

OTAUpdateResult OtaUpdate::RunUpdate()
{
    int RemoteRelease = GetRemoteVersion();

    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    if (RemoteRelease==0)
        return OTA_RELEASE_NUMBER_UNAVAILABLE;
    else
      _newReleaseNumber = RemoteRelease;

      if (RemoteRelease <= _localRelease) {
          Serial.println("Nothing to do!!");
          return OTA_UPDATE_UNNECESSARY;
      }

      ESPhttpUpdate.rebootOnUpdate(false);

      t_httpUpdate_return ret = ESPhttpUpdate.update(_firmwareUrl);

      switch(ret) {
      case HTTP_UPDATE_FAILED:
              Serial.println("[update] Update failed.");
              return OTA_UPDATE_FAILED;

              break;
      case HTTP_UPDATE_NO_UPDATES:
              Serial.println("[update] Update no Update.");
              return OTA_UPDATE_NO_UPDATES;
              break;

      case HTTP_UPDATE_OK:
              Serial.println("[update] Update ok.");
              break;
      }


  return OTA_UPDATE_OK;

}

int OtaUpdate::GetReleaseNumber()
{
    return _newReleaseNumber;

}

int OtaUpdate::GetRemoteVersion()
{
  int RetValue = 0;
  HTTPClient http;
  http.begin(_releaseUrl);
  int httpCode = http.GET();
  if(httpCode > 0)
  {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if(httpCode == HTTP_CODE_OK)
      {
          String payload = http.getString();
          RetValue = payload.toInt();
      }
  }
  else
  {
      RetValue = 0;
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();

  return RetValue;
}
