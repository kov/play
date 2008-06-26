#!/usr/bin/python
# Copyright (C) 2008 Gustavo Noronha Silva
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.  You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

from PyRSS2Gen import RSS2, RSSItem
from StringIO import StringIO
from gzip import GzipFile
from urllib2 import urlopen
from BeautifulSoup import BeautifulSoup
from datetime import datetime, timedelta
import xml.sax.saxutils
import pickle

base_uri = 'http://www.giantitp.com'
uri = 'http://www.giantitp.com/comics/oots.html'

def get_gziped_page_data(uri):
    gzdata = urlopen(uri).read()
    return GzipFile(fileobj = StringIO(gzdata)).read()

def get_page_data(uri):
    data = urlopen(uri).read()
    if not data.startswith('<HTML'):
        data = get_gziped_page_data(uri)
    return data

def get_image_for_uri(uri):
    data = get_page_data(uri)
    if not data.startswith('<HTML'):
        data = get_gziped_page_data(uri)

    all_images = BeautifulSoup(data).findAll('img')
    for image in all_images:
        src = image.get('src', None)
        if src and src.startswith('/comics/images'):
            return '<![CDATA[<img src="' + base_uri + src + '" />]]>'
    return ''

data = get_page_data(uri)
all_links = BeautifulSoup(data).findAll('a')

links = []
links_found = {}
for link in all_links:
    href = link.get('href', None)
    if href and href.startswith('/comics/oots'):
        link_uri = base_uri + href

        # link has already been processed
        if links_found.has_key(link_uri):
            continue

        image = get_image_for_uri(link_uri)
        links.append((link_uri, RSSItem(title = link.contents[0], link = link_uri,
                                        description = image,
                                        pubDate = datetime.now())))
        links_found[link_uri] = True
    # if we already found 10 entries, let's get out of this thing
    if len(links_found) > 10:
        break
links.reverse()

# do we have a cache already?
try:
    cache_file = open('links.cache')
    cached_data = pickle.load(cache_file)
    cache_file.close()
except IOError:
    cached_data = {}

# find items we can already use from the cache
items = []
for count, (uri, item) in enumerate(links):
    if cached_data.has_key(uri):
        cached_item = cached_data[uri]
        items.append(RSSItem(**cached_item))
    else:
        items.append(item)
        # we will only really create entries for new stuff; make sure
        # we have their pubDate at least one minute (some readers seem
        # to not consider seconds) more for each of the newer stuff,
        # so that the order of the comics will be correct
        item.pubDate += timedelta(0.0007 * (count+1))
        cached_data[uri] = dict(title = str(item.title), link = str(item.link),
                                description = str(item.description),
                                pubDate = item.pubDate)

# generate the RSS here
rss = RSS2(title = 'The Order of the Stick',
           link = uri,
           description = 'The Order of the Stick',
           lastBuildDate = datetime.now(),
           items = items)
data = rss.to_xml('utf-8')
open('ots-rss.xml', 'w').write(xml.sax.saxutils.unescape(data))

# update cache
cache_file = open('links.cache', 'w')
pickle.dump(cached_data, cache_file)
cache_file.close()

