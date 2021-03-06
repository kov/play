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
#include <string>
#include "http-request.h"

using namespace std;

HttpRequest::HttpRequest()
{
}

HttpRequest::HttpRequest(string method)
  : method(method)
{
}

HttpRequest::HttpRequest(string method, string uri)
  : method(method), uri(uri)
{
}

HttpRequest::~HttpRequest()
{
  cout << "Destructor called!" << endl << flush;
}

string HttpRequest::getMethod()
{
  return method;
}

void HttpRequest::setMethod(string method)
{
  this->method = method;
}

string HttpRequest::getURI()
{
  return uri;
}

void HttpRequest::setURI(string uri)
{
  this->uri = uri;
}
