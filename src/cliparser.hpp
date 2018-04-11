#ifndef CLIPARSER_HPP
#define CLIPARSER_HPP

#include <QStringList>

enum CliParseResult {
  cliOK,
  cliRedefinition,
  cliValueError,
  cliMissingValue,
  cliUnknownOption,
  cliHelp
};

struct CliOptions {
  QString input;
  QString profile;
  uint threads;
  bool gui;
  bool sortExif;

  CliOptions();
  CliOptions& operator=(const CliOptions& rhs);
};

class CliParser {
public:
  CliParser(int argc, char *argv[]);
  CliParseResult parse(CliOptions &opt);

  QString getCulpritArgument();

private:
  bool next();

  // returns false if head starts with a `-`
  // and thus is an option, and not a value
  bool isValue();

  // argument list
  QStringList m_argv;
  // parser head
  QString m_head;
  // parser position
  uint m_pos;
  // argument that resulted in error
  QString m_errArg;
  // if parse failed
  bool m_errFlag = false;
};

#endif // CLIPARSER_HPP
