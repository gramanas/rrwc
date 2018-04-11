///////////////////////////////////////////////////////////////////////////
// resize-rename-watermark-comment utility for photographers             //
// Copyright (C) 2018  Grammenos Anastasis                               //
//                                                                       //
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation, either version 3 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details.                          //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program.  If not, see <http://www.gnu.org/licenses/>. //
///////////////////////////////////////////////////////////////////////////

#include <QString>
#include <QApplication>
#include <iostream>
#include <thread>

#include "gui/mainwindow.hpp"
#include "rrwc.hpp"
#include "tui.hpp"
#include "cliparser.hpp"

QCoreApplication *initialize(int &argc, char *argv[], bool gui) {
  if (gui) {
    return new QApplication(argc, argv);
  }
  return new QCoreApplication(argc, argv);
}

void handleErrorOrHelp(CliParseResult res, const QString &errArg) {
  if (res == cliHelp) {
    std::cout << "Printing help:...\n";
    return;
  }
  std::cout << "Command line error:\n";
  switch(res) {
  case cliRedefinition:
    std::cout << "Redefinition of " << errArg.toStdString() << std::endl;
    break;
  case cliMissingValue:
    std::cout << "A valid value must follow " << errArg.toStdString() << std::endl;
    break;
  case cliValueError:
    std::cout << errArg.toStdString() << " is not a valid value." << std::endl;
    break;
  case cliUnknownOption:
    std::cout << "Option " << errArg.toStdString() << " not recognized.\nUse -h or --help for help." << std::endl;
    break;
  default:
    return;
  }
}

void setThreads(CliOptions opt) {
  // set number of threads
  if (opt.threads > 0 && opt.threads < 42) {
    return;
  }
  else {
    uint t = std::thread::hardware_concurrency();
    if (t == 0) {
      opt.threads = 1;
    }
    else
      opt.threads = t;
  }
}

int main(int argc, char *argv[]) {
  CliParser p(argc, argv);
  CliOptions opt;
  auto result = p.parse(opt);
  if (result != cliOK) {
    handleErrorOrHelp(result, p.getCulpritArgument());
    return 1;
  }

  QScopedPointer<QCoreApplication> app(initialize(argc, argv, opt.gui));
  Rrwc rrwc;

  if (qobject_cast<QApplication *>(app.data())) {
    MainWindow w(0, &rrwc, opt);
    w.show();
    return app->exec();
  }
  else {
    Tui tui(&rrwc, opt);
    tui.exec();
    std::cout << "cli\n";
    return app->exec();
  }

}
