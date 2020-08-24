const fs = require('fs')
const request = require('request')
const readline = require('readline')

class TaskTree {
    constructor() {
        this.data = {}
        this.loadTasksFile().then(data => {
            this.data = data
            console.log(this.data)
        })
    }

    loadTasksFile() {
        return new Promise((resolve, reject) => {
            var data = {}
            var lineReader = readline.createInterface({
                input: require('fs').createReadStream('/home/kov/.gtimelog/web-tasks.txt')
            })
    
            lineReader.on('line', function(line) {
                if (line.startsWith('#')) {
                    return
                }
    
                var parts = line.trim().split(': ')
                var client = parts[0]
                var project = parts[1]
                var category = parts[2]
                var activity = parts[3]
    
                if (project.startsWith('*')) {
                    return
                }
    
                function ensureKey(table, key) {
                    if (table[key] == undefined) {
                        table[key] = {}
                    }
                }

                ensureKey(data, client)
                ensureKey(data[client], project)
                ensureKey(data[client][project], category)
                ensureKey(data[client][project][category], activity)
            })
    
            lineReader.on('close', function() {
                return resolve(data)
            })
        })
    }
}

class Chronophage {
    async downloadToFile(path, filename) {
        request.get(`https://dev.chronophage.collabora.com${path}?user=kov&auth_token=7oLgPhPiEca3BWIPtnKG`,
        { 'auth': Chronophage.authData })
        .on('response', function(response) {
            if (response.statusCode == 401) {
                console.log('Failed to authenticate.')
            } else if (response.statusCode != 200) {
                console.log('Failed with status code ' + response.statusCode)
            }
            else {
                console.log(response.toJSON())
            }
        })
        .on('error', function(error) {
            console.log(error)
        })
        .pipe(fs.createWriteStream(filename))
    }

    getTaskList() {
        this.downloadToFile('/tasks', '/home/kov/.gtimelog/web-tasks.txt')
    }
}

Chronophage.authData = {
    'user': 'kov',
    'pass': 'Beatowbefttobseik3',
    'sendImmediately': true
}

module.exports = {
    'Chronophage': Chronophage,
    'TaskTree': TaskTree
}