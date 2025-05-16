const clock = document.querySelector('div#clock');

window.addEventListener('load', async e => {
    setInterval(() => {
        const now = new Date();
        const hours = String(now.getHours()).padStart(2, '0');
        const minutes = String(now.getMinutes()).padStart(2, '0');
        const seconds = String(now.getSeconds()).padStart(2, '0');
        clock.innerText = `${hours}:${minutes}:${seconds}`;
    }, 50);

    navigator.serviceWorker.register('service.js');
});