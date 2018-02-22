const { app, BrowserWindow } = require('electron');
const path = require('path');
const url = require('url');
const fs = require('fs');

const args = process.argv;

console.log( args );

function help() {
  process.stdout.write( ". -m=h:/bin/xdhqxdh/ <dir>");
  process.exit(-1 );
}

if ( args.length < 4 )
  help();
else if (args[3][0] == '-' )
  help();

const cdnPath = path.posix.join( args[3], '/' );

console.log( cdnPath );


/*

if ( process.env.EPEIOS_SRC ) {
 var addonPath = null;
 if ( process.platform == 'win32' )
  addonPath = 'h:/bin/';
 else
  addonPath = '~/bin/';
 const xdhelcq = require( addonPath + 'xdhelcq.node');
} else {
 const binary = require( 'node-pre-gyp' );
 const path = require( 'path' );
 const xdhelcq_path = binary.find( path.resolve( path.join( __dirname, './package.json' ) ) );
 xdhelcq = require( xdhelcq_path );	
}

*/
/*
const xdhelcq = require( 'h:\\bin\\xdhelcq.node');

console.log( xdhelcq.wrapperInfo() );
*/

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let win;

function createWindow() {
  // Create the browser window.
  win = new BrowserWindow({ width: 800, height: 600 })

  // and load the index.html of the app.
  /*
  win.loadURL(url.format({
    pathname: path.join(__dirname, 'XDHELCq.html'),
    protocol: 'file:',
    slashes: true
  }))
  */
  
   // Open the DevTools.
  win.webContents.openDevTools();
  
  console.log( cdnPath );


  let head = fs.readFileSync( path.join( cdnPath, "head.html") );
  let html = fs.readFileSync( path.join( __dirname, "XDHELCq.html" ), "utf8").replace( "<!-- $USER_HEAD -->", head );

//  console.log( html );

  win.loadURL("data:text/html;charset=utf-8," + encodeURI(html), { baseURLForDataURL: "file://" + cdnPath });
//  win.loadURL("data:text/html;charset=utf-8," + encodeURI(html), { baseURLForDataURL: "file://h:/hg/epeios/tools/xdhq/examples/common/blank/" });

  //  win.loadURL("data:text/html;charset=utf-8," + encodeURI(html));

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
