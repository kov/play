const electron = require('electron')
const request = require('request')

const auth = {
    'user': 'kov',
    'pass': 'Beatowbefttobseik3',
    'sendImmediately': true
}

request.get('https://dev.chronophage.collabora.com/', {'auth': auth}, function (error, response, body) {
    console.log('error:', error)
    console.log('status code:', response.statusCode)
})

function createWindow() {
    var win = new electron.BrowserWindow({width: 800, height: 600})
    win.setMenu(null)
    win.loadFile('index.html')
}

electron.app.on('ready', createWindow)

electron.app.on('window-all-closed', () => {
    electron.app.quit()
})