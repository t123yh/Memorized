#include "database.h"
#include "utils.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QtSql>

void
executeQueriesFromFile(QFile& file, QSqlQuery& query)
{
  while (!file.atEnd()) {
    QByteArray readLine = "";
    QString cleanedLine;
    QString line = "";
    bool finished = false;
    while (!finished) {
      readLine = file.readLine();
      cleanedLine = readLine.trimmed();
      // remove comments at end of line
      QStringList strings = cleanedLine.split("--");
      cleanedLine = strings.at(0);

      // remove lines with only comment, and DROP lines
      if (!cleanedLine.startsWith("--") && !cleanedLine.startsWith("DROP") &&
          !cleanedLine.isEmpty()) {
        line += cleanedLine;
      }
      if (cleanedLine.endsWith(";")) {
        break;
      }
      if (cleanedLine.startsWith("COMMIT")) {
        finished = true;
      }
    }

    if (!line.isEmpty()) {
      query.exec(line);
    }
    if (!query.isActive()) {
      Crash(query.lastError().text());
    }
  }
}

static void
PopulateDb()
{
  QFile f(":/miscellaneous/res/initdb.sql");
  if (!f.open(QFile::ReadOnly | QFile::Text)) {
    Crash("Unable to read database population file.");
  }
  QSqlQuery query;
  executeQueriesFromFile(f, query);
}

void
InitDb(const QString& dbName)
{
  if (!QSqlDatabase::drivers().contains("QSQLITE")) {
    Crash("We need SQLite driver!");
  }

  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbName);

  if (!db.open()) {
    Crash(QString("Db error: %1").arg(db.lastError().text()));
  }

  if (db.exec("PRAGMA foreign_keys = ON;").lastError().isValid()) {
    Crash(QString("Unable to turn on foreign key constraint."));
  }

  QStringList tables = db.tables();
  if (!tables.contains("cards", Qt::CaseInsensitive)) {
    PopulateDb();
  }
}
