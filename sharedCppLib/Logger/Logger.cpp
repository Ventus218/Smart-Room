#include <Logger.h>
#include <utils.h>

bool Logger::hasBeenInitialized = false;
LogLevel Logger::logLevel = LogLevel::INFO;

void Logger::init(unsigned long baud, LogLevel logLevel) {
    if (Logger::hasBeenInitialized) {
        crashWithError(F("Logger has already been initialized..."));
    }

    Serial.begin(baud);
    while (!Serial){}
    
    Logger::logLevel = logLevel;
    Logger::hasBeenInitialized = true;
}

String Logger::logLevelToString(LogLevel logLevel) {
    String str;

    switch (logLevel) {
        case LogLevel::INFO:
            str = F("INFO");
            break;
        case LogLevel::NOTICE:
            str = F("NOTICE");
            break;
        case LogLevel::WARNING:
            str = F("WARNING");
            break;
        case LogLevel::ERROR:
            str = F("ERROR");
            break;
    }

    return str;
}

void Logger::log(String message) {
    if (Logger::hasBeenInitialized) {
        Serial.println(message);
        Serial.flush();
    }
}

void Logger::log(String message, LogLevel logLevel) {
    if (Logger::hasBeenInitialized && logLevel >= Logger::logLevel) {
        Serial.println(logLevelToString(logLevel) + ":\t" + message);
        Serial.flush();
    }
}
