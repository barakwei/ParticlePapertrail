#pragma once

#include "application.h"

class PapertrailLogHandler : public LogHandler {
  UDP m_udp;
  String m_host;
  uint16_t m_port;
  String m_app;
  bool m_inited;

public:
    explicit PapertrailLogHandler(String host, uint16_t port, String app, LogLevel level = LOG_LEVEL_INFO, const Filters &filters = {});
    virtual ~PapertrailLogHandler();

private:
    const char* extractFileName(const char *s);
    const char* extractFuncName(const char *s, size_t *size);
    void log(String message);
    static const uint16_t kLocalPort;

protected:
    virtual void logMessage(const char *msg, LogLevel level,
        const char *category, const LogAttributes &attr) override;
};
