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
#include <fstream>
#include <string>

using namespace std;

void copy_fstab()
{
  ifstream in;
  ofstream out;
  string buffer;

  in.open("/etc/fstab");
  out.open("/tmp/fstab");
  
  while(not in.eof())
    {
      getline(in, buffer);
      out << buffer << endl;
    }

  in.close();
  out.close();
}

int main(int argc, char **argv)
{
  int age = 24;

  cout << "Your age in hex: " << hex << age << endl;
  cout << "There are also some " << uppercase << "other manipulators to apply." << endl;

  copy_fstab();

  return 0;
}
