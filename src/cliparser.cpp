#include <QFile>
#include <QDebug>

#include "cliparser.hpp"


CliOptions::CliOptions() {
  input = profile = "";
  gui = sortExif = true;
  threads = 0;
}

CliOptions& CliOptions::operator=(const CliOptions& rhs) {
  if (this != &rhs) {
    this->input = rhs.input;
    this->profile = rhs.profile;
    this->threads = rhs.threads;
    this->gui = rhs.gui;
    this->sortExif = rhs.sortExif;
  }
  return *this;
}


CliParser::CliParser(int argc, char *argv[]) {
  for (int i=0; i<argc; i++) {
    m_argv << argv[i];
  }

  // head at program name
  m_pos = 0;
  m_head = m_argv[m_pos];
}

bool CliParser::next() {
  if (m_pos < m_argv.size()-1) {
    m_head = m_argv[++m_pos];
    return true;
  }
  // end of the line
  return false;
}

bool CliParser::isValue() {
  if (m_head.startsWith('-')) {
    return false;
  }
  return true;
}

QString CliParser::getCulpritArgument() {
  if (m_errFlag) {
    return m_errArg;
  }
  return QString();
}

CliParseResult CliParser::parse(CliOptions &opt) {
  CliOptions _opt;
  // skip program name
  while (next()) {
    // set errArg here in case parse errors
    m_errArg = m_head;
    if (m_head == "-i" || m_head == "--input") {
      if (_opt.input.isEmpty()) {
        // skip indicator
        if (!next()) {
          m_errFlag = true;
          return cliMissingValue;
        }
        if (!isValue()) {
          m_errFlag = true;
          // the error is in the value
          m_errArg = m_head;
          return cliValueError;
        }
        _opt.input = m_head;
      }
      else {
        // redefiniton
        m_errFlag = true;
        return cliRedefinition;
      }
    }
    else if (m_head == "-p" || m_head == "--profile") {
      if (_opt.profile.isEmpty()) {
        // skip indicator
        if (!next()) {
          m_errFlag = true;
          return cliMissingValue;
        }
        if (!isValue()) {
          m_errFlag = true;
          // the error is in the value
          m_errArg = m_head;
          return cliValueError;
        }
        _opt.profile = m_head;
      }
      else {
        // redefiniton
        m_errFlag = true;
        return cliRedefinition;
      }
    }
    else if (m_head == "-nw" || m_head == "--headless") {
      if (_opt.gui) {
        _opt.gui = false;
      }
      else {
        // redefiniton
        m_errFlag = true;
        return cliRedefinition;
      }
    }
    else if (m_head == "-sf" || m_head == "--sort-filename") {
      if (_opt.sortExif) {
        _opt.sortExif = false;
      }
      else {
        // redefiniton
        m_errFlag = true;
        return cliRedefinition;
      }
    }
    else if (m_head == "-t" | m_head == "--threads") {
      if (_opt.threads == 0) {
        // skip indicator
        if (!next()) {
          m_errFlag = true;
          return cliMissingValue;
        }
        bool ok;
        uint n = m_head.toUInt(&ok);
        if (!ok) {
          m_errFlag = true;
          // the error is in the value
          m_errArg = m_head;
          return cliValueError;
        }
        _opt.threads = n;
      } else {
        // redefiniton
        m_errFlag = true;
        return cliRedefinition;
      }
    }
    else if (m_head == "-h" | m_head == "--help") {
      return cliHelp;
    }
    else {
      m_errFlag = true;
      return cliUnknownOption;
    }
  }

  // assign the values
  opt = _opt;
  return cliOK;
}
