const electron = require('electron')
const chronophage = require('./chronophage')

function createWindow() {
    var win = new electron.BrowserWindow({width: 800, height: 600})
    win.setMenu(null)
    win.loadFile('index.html')
}

function loadTaskTree() {
    var taskTree = new chronophage.TaskTree
}

function initialize() {
    createWindow()
    var chrono = new chronophage.Chronophage
    let taskList = chrono.getTaskList()

    setTimeout(loadTaskTree, 10000)
    console.log('kov')
}

electron.app.on('ready', initialize)

electron.app.on('window-all-closed', () => {
    electron.app.quit()
})
