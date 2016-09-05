#ifndef OTAUPDATE_H
#define OTAUPDATE_H

enum OTAUpdateResult {
    OTA_UPDATE_FAILED,
    OTA_UPDATE_OK,
    OTA_RELEASE_NUMBER_UNAVAILABLE,
    OTA_UPDATE_UNNECESSARY,
    OTA_UPDATE_NO_UPDATES
};

class OtaUpdate
{
  public:
    OtaUpdate(String firmwareUrl, int localRelease);
    OTAUpdateResult RunUpdate();
    int GetReleaseNumber();
    void Reboot();

  private:
    int GetRemoteVersion();
    String _firmwareUrl;
    String _releaseUrl;
    int _localRelease;
    int _newReleaseNumber;
};


#endif
