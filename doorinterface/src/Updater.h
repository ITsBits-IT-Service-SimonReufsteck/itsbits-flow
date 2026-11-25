#pragma once

#include <Arduino.h>

class Updater {
public:
    enum State { IDLE, CHECKING, UPDATE_AVAILABLE, DOWNLOADING, DONE, FAILED };

    void begin();
    void loop();
    void checkForUpdate();
    void checkForUpdate(const String& board, const String& channel);
    void startUpdate();
    void setAutoUpdateEnabled(bool enabled);
    void setAutoUpdateHour(int hour);
    void setTimezone(const String& tz);
    String getStatusJson();

private:
    State _state = IDLE;
    String _latest = "";
    String _downloadUrl = "";
    size_t _progress = 0;
    size_t _total = 0;
    String _error = "";
    String _requestedBoard = "";
    String _requestedChannel = "";
    bool _autoUpdateEnabled = false;
    int _autoUpdateHour = 5;
    String _timezone = "Europe/Berlin";
    unsigned long _lastUpdateCheck = 0;

    void setState(State s);
    void fail(const String& msg);
    void doCheck();
    void doDownload();
    String extractJsonField(const String& json, const String& key);
    String stripFwPrefix(const String& tag);
    String buildAssetName(const String& version, const String& channel, const String& board);
    bool shouldCheckAutoUpdate();
};