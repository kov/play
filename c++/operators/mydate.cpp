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

#include <QTextStream>
#include "mydate.h"

QDate operator-(const QDate& date_left, const QDate& date_right)
{
  QDate beginning_of_time = QDate(1, 1, 1);
  QTextStream cout(stdout, QIODevice::WriteOnly);
  int days_right;

  cout << beginning_of_time.toString() << endl;
  days_right = date_right.daysTo(beginning_of_time);

  return date_left.addDays(days_right);
}

