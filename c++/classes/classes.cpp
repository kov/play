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
#include "http-request.h"

using namespace std;

void print_request(HttpRequest& request)
{
  cout << request.getMethod() << endl << flush;
  cout << request.getURI() << endl << flush;
}

int main()
{
  HttpRequest request;

  request.setMethod("GET");
  request.setURI("/test");

  print_request(request);

  HttpRequest other_request("POST", "/blah");

  print_request(other_request);

  return 0;
}
