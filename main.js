const {app, BrowserWindow} = require('electron')

const url = require('url')
const path = require('path')
const { exit } = require('process')

let win

function createWindow() {
  
   win = new BrowserWindow({show: false, icon:__dirname+'/files/logo.ico'})
   win.setMenu(null)
   win.webSecurity= false;
   win.maximize();
win.show();
   
   
   win.loadURL(url.format ({
      pathname: path.join(__dirname, 'index.html'),
      protocol: 'file:',
      slashes: true
   }))
   win.on('closed', () =>{
       win = null
   });
}

app.on('ready', createWindow)