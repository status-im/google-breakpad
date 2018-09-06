#ifndef EXCEPTION_GLOBAL_HANDLER
#define EXCEPTION_GLOBAL_HANDLER

#include <QString>

namespace google_breakpad {
    class ExceptionHandler;
}

typedef void (*ExceptionPostHandledCallback)();

class ExceptionGlobalHandler
{
public:
    ExceptionGlobalHandler(const QString& crashReporterPath,
                           ExceptionPostHandledCallback postHandledCallback = nullptr,
                           const QString& logsPath = "");
    ~ExceptionGlobalHandler();

private:
    void initilize();
    google_breakpad::ExceptionHandler *breakpadHandler = nullptr;
};

#endif // EXCEPTION_GLOBAL_HANDLER

