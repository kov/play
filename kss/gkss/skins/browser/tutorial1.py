#!/usr/bin/env python
# -*- coding: utf-8; -*-
#
# Copyright (C) 2008 AlfaiaTI Tecnologia sob medida
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

from zope.interface import implements
from kss.core import KSSView, kssaction
from datetime import datetime

class TutorialView(KSSView):
    @kssaction
    def response1(self, message):
        content = u'Agora são: %s (%s)' % (str(datetime.now()), message)
        core = self.getCommandSet('core')
        core.replaceInnerHTML('#portal-siteactions', content)

