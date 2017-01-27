#include "papertrail.h"

// TODO: Change the host and ports to match yours.
// https://github.com/barakwei/ParticlePapertrail for more information.
PapertrailLogHandler papertailHandler("logsX.papertrailapp.com", 12345, "PapertrailSimpleDemo");

int i;

void setup() {
}

void loop() {
  Log.info("Log message #%d", ++i);
  Log.warn("This is warning message");
  Log.error("This is error message");
  delay(5000);
}
