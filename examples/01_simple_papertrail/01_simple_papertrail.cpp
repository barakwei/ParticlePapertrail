// Log system and application messages to Papertrail
//
// See https://github.com/barakwei/ParticlePapertrail for setup information
// See https://docs.particle.io/reference/firmware/#logging for the firmware Logging API
#include "papertrail.h"

// TODO: Change the host and ports to match yours.
PapertrailLogHandler papertailHandler("logsX.papertrailapp.com", 12345, "PapertrailSimpleDemo");

#if PLATFORM_ID == 10
#warning "Logging on the Electron consumes a lot of data. Be careful about how often you log."
#endif

int i = 0;

void setup() {
}

void loop() {
  Log.info("Log message #%d", ++i);
  Log.warn("This is warning message");
  Log.error("This is error message");
  delay(5000);
}
