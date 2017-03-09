#include "papertrail.h"

///Local port to be used by the socket.
const uint16_t PapertrailLogHandler::kLocalPort = 8888;

PapertrailLogHandler::PapertrailLogHandler(String host, uint16_t port, String app, String system, LogLevel level,
    const LogCategoryFilters &filters) : LogHandler(level, filters), m_host(host), m_port(port), m_app(app),
                                         m_system(system)  {
    m_inited = false;
    LogManager::instance()->addHandler(this);
}

IPAddress PapertrailLogHandler::resolve(const char *host) {
#if Wiring_WiFi
    return WiFi.resolve(host);
#elif Wiring_Cellular
    return Cellular.resolve(host);
#else
#error Unsupported plaform
#endif
}

/// Send the log message to Papertrail.
void PapertrailLogHandler::log(String message) {
    String time = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);
    String packet = String::format("<22>1 %s %s %s - - - %s", time.c_str(), m_system.c_str(), m_app.c_str(),
                                   message.c_str());
    int ret = m_udp.sendPacket(packet, packet.length(), m_address, m_port);
    if (ret < 1) {
        m_inited = false;
    }
}

PapertrailLogHandler::~PapertrailLogHandler() {
    LogManager::instance()->removeHandler(this);
}

/// Initialize socket and resolve address if needed.
bool PapertrailLogHandler::lazyInit() {
    if (!m_inited) {
        uint8_t ret = m_udp.begin(kLocalPort);
        m_inited = ret != 0;

        if (!m_inited) {
            return false;
        }
    }

    if (!m_address) {
        m_address = resolve(m_host);

        if (!m_address) {
            return false;
        }
    }

    return true;
}

// The floowing methods are taken from Particle FW, specifically spark::StreamLogHandler.
// See https://github.com/spark/firmware/blob/develop/wiring/src/spark_wiring_logging.cpp
const char* PapertrailLogHandler::PapertrailLogHandler::extractFileName(const char *s) {
    const char *s1 = strrchr(s, '/');
    if (s1) {
        return s1 + 1;
    }
    return s;
}

const char* PapertrailLogHandler::extractFuncName(const char *s, size_t *size) {
    const char *s1 = s;
    for (; *s; ++s) {
        if (*s == ' ') {
            s1 = s + 1; // Skip return type
        } else if (*s == '(') {
            break; // Skip argument types
        }
    }
    *size = s - s1;
    return s1;
}

void PapertrailLogHandler::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
    if (!lazyInit()) {
      return;
    }

    String s;

    if (category) {
        s.concat("[");
        s.concat(category);
        s.concat("] ");
    }

    // Source file
    if (attr.has_file) {
        s = extractFileName(attr.file); // Strip directory path
        s.concat(s); // File name
        if (attr.has_line) {
            s.concat(":");
            s.concat(String(attr.line)); // Line number
        }
        if (attr.has_function) {
            s.concat(", ");
        } else {
            s.concat(": ");
        }
    }

    // Function name
    if (attr.has_function) {
        size_t n = 0;
        s = extractFuncName(attr.function, &n); // Strip argument and return types
        s.concat(s);
        s.concat("(): ");
    }

    // Level
    s.concat(levelName(level));
    s.concat(": ");

    // Message
    if (msg) {
        s.concat(msg);
    }

    // Additional attributes
    if (attr.has_code || attr.has_details) {
        s.concat(" [");
        // Code
        if (attr.has_code) {
            s.concat(String::format("code = %p" , (intptr_t)attr.code));
        }
        // Details
        if (attr.has_details) {
            if (attr.has_code) {
                s.concat(", ");
            }
            s.concat("details = ");
            s.concat(attr.details);
        }
        s.concat(']');
    }

    log(s);
}
