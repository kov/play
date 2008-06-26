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

#import <stdio.h>

#import "hello.h"

@implementation Hello
+ (void) say
{
  printf("Hello World!\n");
}

+ (void) sayString: (char*) str andNumber: (int) num
{
  printf("String: %s\nNumber: %d\n", str, num);
}

- (void) setAge: (int) newAge
{
  age = newAge;
}

- (int) age
{
  return age;
}
@end
