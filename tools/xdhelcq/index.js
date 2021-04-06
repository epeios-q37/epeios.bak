/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

// const devTools = true;  // Uncomment to open dev tools.

const { app, BrowserWindow } = require('electron');
const path = require('path');
const url = require('url');
const fs = require('fs');
 
const args = process.argv;

function getXdhelcqPath() {
  if (process.env.Q37_EPEIOS) {
    let addonPath = null;
    if (process.platform == 'win32')
      addonPath = 'h:/bin/';
    else
      addonPath = '/home/csimon/bin/';
    return require('path').join(addonPath, 'xdhelcq.node');
  } else {
    return require('path').join(__dirname, "xdhelcq.node");
  }
}

function initialize(arguments) {
  var mergedArguments = "";
  var length = arguments.length;

  while (length--) {
    mergedArguments = '"' + arguments[length] + '" ' + mergedArguments;
  }

  console.log( mergedArguments );
  xdhelcq.initialize(mergedArguments);
}

const xdhelcq = require(getXdhelcqPath());
initialize(process.argv);

// console.log( args );

function help() {
  process.stdout.write(". -m=h:/bin/xdhqxdh <dir>");
  process.exit(-1);
}

if (args.length < 4)
  help();
else if (args[args.length - 1][0] == '-')
  help();

const cdnPath = path.posix.join(args[args.length - 1], '/');

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let win;

function createWindow() {
  // Create the browser window.
  win = new BrowserWindow({ width: 800, height: 600 })

  // Open the DevTools.
  if ( typeof devTools !== 'undefined' && devTools)
    win.webContents.openDevTools();

  let html = fs.readFileSync(path.join(__dirname, "XDHELCq.html"), "utf8").replace(/\$XDHELCQ_PATH\$/g, __dirname.replace(/\\/g, "\\\\")).replace(/\$XDHELCQ_HEAD\$/g, xdhelcq.getHead());

  win.loadURL("data:text/html;charset=utf-8," + encodeURI(html), { baseURLForDataURL: "file://" + cdnPath });

  // Emitted when the window is closed.
  win.on('closed', () => {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    win = null
  })
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow)
// console.log( xdhelcq.moduleInfo());
// console.log( xdhelcq.getHead());

// Quit when all windows are closed.
app.on('window-all-closed', () => {
  // On macOS it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  // On macOS it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (win === null) {
    createWindow()
  }
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
