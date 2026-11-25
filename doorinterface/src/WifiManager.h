#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <DNSServer.h>
#include <WebServer.h>

class Updater;

enum PortalState { PORTAL_IDLE, PORTAL_CONNECTING, PORTAL_CONNECTED, PORTAL_FAILED };

class WifiManager {
public:
    void begin();
    void loop();
    bool isConnected();
    bool isApActive();
    String getHostname() { return _hostname; }
    bool setHostname(const String& hostname);
    String getBoardType() { return _boardType; }
    String getFwChannel() { return _fwChannel; }
    bool getAutoUpdateEnabled() { return _autoUpdateEnabled; }
    int getAutoUpdateHour() { return _autoUpdateHour; }
    String getTimezone() { return _timezone; }
    bool setBoardConfig(const String& boardType, const String& channel, bool autoUpdateEnabled, int autoUpdateHour, const String& timezone);
    void setUpdater(Updater* updater) { _updater = updater; }

private:
    Preferences _prefs;
    bool _apActive = false;
    String _ssid;
    String _pass;
    String _hostname;
    String _boardType;
    String _fwChannel;
    bool _autoUpdateEnabled = false;
    int _autoUpdateHour = 5;
    String _timezone = "Europe/Berlin";

    DNSServer _dns;
    WebServer _server{80};
    const byte _dnsPort = 53;

    PortalState _portalState = PORTAL_IDLE;
    unsigned long _connectStart = 0;
    const unsigned long _connectTimeout = 15000;
    unsigned long _connectedAt = 0;
    const unsigned long _apOffDelay = 30000;
    bool _shutdownRequested = false;

    unsigned long _staDownSince = 0;
    unsigned long _lastReconnectAttempt = 0;
    uint32_t _reconnectAttempts = 0;
    static constexpr unsigned long RECONNECT_INTERVAL = 10000;
    static constexpr unsigned long RESTART_AFTER = 300000;

    void loadCredentials();
    void loadHostname();
    bool tryConnect();
    void startFallbackAp();
    void startPortal();
    void pollConnect();
    void shutdownAp();
    void staWatchdog();
    void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

    void handleRoot();
    void handleCss();
    void handleJs();
    void handleScan();
    void handleSave();
    void handleStatus();
    void handleConfig();
    void handleRedirect();
    void handleClose();

    Updater* _updater = nullptr;
};