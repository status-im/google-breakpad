#ifndef EXCEPTION_GLOBAL_HANDLER
#define EXCEPTION_GLOBAL_HANDLER

#include <QString>

namespace google_breakpad {
    class ExceptionHandler;
}

class ExceptionGlobalHandler
{
public:
    ExceptionGlobalHandler(const QString& crashReporterPath);
    ~ExceptionGlobalHandler();

private:
    void initilize();
    google_breakpad::ExceptionHandler *breakpadHandler = nullptr;
};

#endif // EXCEPTION_GLOBAL_HANDLER

