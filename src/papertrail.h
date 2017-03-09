#pragma once

#include "application.h"

#if (SYSTEM_VERSION < SYSTEM_VERSION_v061)
#error This library requires FW version 0.6.1 and above.
#endif

/// LogHandler that send logs to Papertrail (https://papertrailapp.com/). Before using this class it's best to
/// familiarize yourself with Particle's loggin facility https://docs.particle.io/reference/firmware/photon/#logging.
/// You can use this as any other LogHandler - Initialize this class as a global, then call Log.info() and friends.
class PapertrailLogHandler : public LogHandler {
    String m_host;
    uint16_t m_port;
    String m_app;
    String m_system;
    UDP m_udp;
    bool m_inited;
    IPAddress m_address;

public:
    /// Initialize the log handler.
    /// \param host Hostname of the Papertrail log server.
    /// \param port Port of the Papertrail log server.
    /// \param app The name of the application in every log entry.
    /// \param system The name of the system in every log entry. Defaults to the deviceID.
    /// \para level Default log level.
    /// \param filters Category filters.
    ///
    /// Each log entry uses RFC 5424 with the following format:
    /// "<22>1 %ISO8601_TIME% %system% %app% - - - [%category%] %log_level%: %text".
    explicit PapertrailLogHandler(String host, uint16_t port, String app, String system = System.deviceID(),
                                  LogLevel level = LOG_LEVEL_INFO, const LogCategoryFilters &filters = {});
    virtual ~PapertrailLogHandler();

private:

    bool lazyInit();
    const char* extractFileName(const char *s);
    const char* extractFuncName(const char *s, size_t *size);
    void log(String message);
    static IPAddress resolve(const char *host);
    static const uint16_t kLocalPort;

protected:
    virtual void logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) override;
};
