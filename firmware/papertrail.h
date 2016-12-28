#pragma once

#include "application.h"

#if (SYSTEM_VERSION < 0x00060000)
#error This library requires FW version 0.6.0 and above.
#endif

class PapertrailLogHandler : public LogHandler {
    String m_host;
    uint16_t m_port;
    String m_app;
    UDP m_udp;
    bool m_inited;
    IPAddress m_address;

public:
    explicit PapertrailLogHandler(String host, uint16_t port, String app, LogLevel level = LOG_LEVEL_INFO, const Filters &filters = {});
    virtual ~PapertrailLogHandler();

private:

    bool lazyInit();
    const char* extractFileName(const char *s);
    const char* extractFuncName(const char *s, size_t *size);
    void log(String message);
    static IPAddress resolve(const char *host);
    static const uint16_t kLocalPort;

protected:
    virtual void logMessage(const char *msg, LogLevel level,
        const char *category, const LogAttributes &attr) override;
};
