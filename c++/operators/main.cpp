/*
 * Copyright (C) 2008 Gustavo Noronha Silva
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.  You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include <iostream>
#include <QApplication>
#include <QTextStream>

#include "mydate.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QTextStream cout(stdout, QIODevice::WriteOnly);
  QDate now, birthday, ret;

  now = QDate::currentDate();
  cout << now.toString() << endl;
  birthday = QDate(now.year(), 6, 13);
  cout << birthday.toString() << endl;

  ret = birthday - now;
  cout << "Difference: " << ret.toString() << endl;

  return 0;
}
