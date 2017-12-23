#include "database.h"
#include "mainwindow.h"
#include <QApplication>

void
myMessageOutput(QtMsgType type,
                const QMessageLogContext& context,
                const QString& msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
    case QtDebugMsg:
      fprintf(stderr,
              "Debug: %s (%s:%u, %s)\n",
              localMsg.constData(),
              context.file,
              context.line,
              context.function);
      break;
    case QtInfoMsg:
      fprintf(stderr,
              "Info: %s (%s:%u, %s)\n",
              localMsg.constData(),
              context.file,
              context.line,
              context.function);
      break;
    case QtWarningMsg:
      fprintf(stderr,
              "Warning: %s (%s:%u, %s)\n",
              localMsg.constData(),
              context.file,
              context.line,
              context.function);
      break;
    case QtCriticalMsg:
      fprintf(stderr,
              "Critical: %s (%s:%u, %s)\n",
              localMsg.constData(),
              context.file,
              context.line,
              context.function);
      break;
    case QtFatalMsg:
      fprintf(stderr,
              "Fatal: %s (%s:%u, %s)\n",
              localMsg.constData(),
              context.file,
              context.line,
              context.function);
      abort();
  }
}

int
main(int argc, char* argv[])
{
  QFont font("Microsoft Yahei UI");
  font.setPointSize(9);
  qInstallMessageHandler(myMessageOutput);
  InitDb("db.db");

  QFile styleFile(":/miscellaneous/res/style.qss");
  styleFile.open(QFile::ReadOnly);
  QString style(styleFile.readAll());

  QApplication a(argc, argv);

  a.setStyleSheet(style);
  a.setFont(font);
  MainWindow w;
  w.show();
  w.setStyleSheet(style);

  return a.exec();
}
