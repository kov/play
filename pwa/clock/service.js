const assets = [
    './',
    './app.js',
]

self.addEventListener('install', async e => {
    const cache = await caches.open('assets');
    cache.addAll(assets);
});

self.addEventListener('fetch', e => {
    const req = e.request;
    const cached = caches.match(req);
    e.respondWith(cached || fetch(req));
});