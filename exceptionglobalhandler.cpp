#include "exceptionglobalhandler.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>

#ifdef Q_OS_LINUX
#include "client/linux/handler/exception_handler.h"
#elif defined(Q_OS_MACOS)
#include "client/mac/handler/exception_handler.h"
#elif defined(Q_OS_WIN)
#include "client/windows/handler/exception_handler.h"
#endif

QString g_crashReporterPath;
ExceptionPostHandledCallback g_exceptionPostHandledCallback = nullptr;
QString g_logsPath;

bool handleException(QString path) {
  const QStringList argumentList = {
      path, QCoreApplication::applicationFilePath(),
      QCoreApplication::applicationName(),
      QCoreApplication::applicationVersion(), g_logsPath};

  qDebug() << "Hander process path:" << g_crashReporterPath;
  qDebug() << "Crash handler arguments list: " << argumentList;
  QProcess::startDetached(g_crashReporterPath, argumentList);

  if (g_exceptionPostHandledCallback) {
    g_exceptionPostHandledCallback();
  }
  return true;
}

#ifdef Q_OS_LINUX
bool minidumpReadyCallback(
    const google_breakpad::MinidumpDescriptor &descriptor, void *,
    bool succeeded) {
  if (!succeeded)
    return false;

  return handleException(QString::fromLocal8Bit(descriptor.path()));
}
#elif defined(Q_OS_MACOS)
bool minidumpReadyCallback(const char *dump_dir, const char *minidump_id,
                           void *context, bool succeeded) {
  Q_UNUSED(context);

  if (!succeeded)
    return false;

  const QString path = QString::fromLocal8Bit(dump_dir) + '/' +
                       QString::fromLocal8Bit(minidump_id) + ".dmp";

  return handleException(path);
}
#elif defined(Q_OS_WIN)
bool minidumpReadyCallback(const wchar_t* dump_dir, const wchar_t* minidump_id,
                           void *context, EXCEPTION_POINTERS* exinfo,
                           MDRawAssertionInfo* assertion, bool succeeded) {
  Q_UNUSED(exinfo);
  Q_UNUSED(assertion);

  if (!succeeded)
    return false;

  QDir minidumpDir = QDir(QString::fromWCharArray(dump_dir));
  QString minidumpFileName = minidumpDir.absoluteFilePath(QString::fromWCharArray(minidump_id) + ".dmp");
  return handleException(minidumpFileName);
}
#endif

ExceptionGlobalHandler::ExceptionGlobalHandler(
    const QString &crashReporterPath,
    ExceptionPostHandledCallback postHandledCallback, const QString &logsPath) {
  g_crashReporterPath = crashReporterPath;
  g_exceptionPostHandledCallback = postHandledCallback;
  g_logsPath = logsPath;
  initilize();
}

void ExceptionGlobalHandler::initilize() {
#ifdef Q_OS_LINUX
  breakpadHandler = new google_breakpad::ExceptionHandler(
      google_breakpad::MinidumpDescriptor(QDir::tempPath().toStdString()), NULL,
      minidumpReadyCallback, NULL, true, -1);
#elif defined(Q_OS_MACOS)
  breakpadHandler = new google_breakpad::ExceptionHandler(
      QDir::tempPath().toStdString(), nullptr, minidumpReadyCallback, NULL,
      true, NULL);
#elif defined(Q_OS_WIN)
  std::wstring pathStr = (const wchar_t*)QDir::tempPath().utf16();
  breakpadHandler = new google_breakpad::ExceptionHandler(
        pathStr, nullptr, minidumpReadyCallback, nullptr, true);
#endif
}

ExceptionGlobalHandler::~ExceptionGlobalHandler() {
  if (breakpadHandler) {
    delete breakpadHandler;
    breakpadHandler = nullptr;
  }
}
