const { app, BrowserWindow } = require('electron')
const path = require('path')
const url = require('url')

console.log("!!!!", process.argv);

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
let win

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

  let html = [
    '<!DOCTYPE html>',
    '<html>',
    ' <head>',
    ' <meta charset="UTF-8" />',
    '	<meta http-equiv="X-UA-Compatible" content="IE=edge" />',
    '	<script src="file://h:/hg/epeios/corpus/js/xdhtml.js"></script>',
    ' <script>',
    ' function attachDebugger()',
    ' {',
	  '  alert( "Attach debugger !" );',
    ' }',
    ' function log( message )',
    ' {',
    '  console.log( message );',
    ' }',
    ' function launchEvent( digest )',
    ' {',
    '  xdhelcq.launchEvent( digest );',
    ' }',
    ' function initialize( arguments ) {',
    '  var mergedArguments = "";',
    '  var length = arguments.length;',
    '',
    '   while ( length-- ) {',
    ' 	 mergedArguments = \'"\' + arguments[length] + \'" \' + mergedArguments;',
    '	  }',
    '',
    '  //	console.log( mergedArguments );',
    '	 xdhelcq.initialize( mergedArguments );',
    ' }',
    'function userHead() {',
    ' try {',
    '  return require(fs) .readFileSync("head.html");',
    ' } catch (err) {',
    '  return "";',
    ' }',
    '}',
    '',
    '</script>',
    '</head>',
    '<body id="XDHRoot">',
    '<script>',
    'attachDebugger();',
    '',
    'var xdhelcqPath = null;',
    '',
    'if ( process.env.EPEIOS_SRC ) {',
    ' var addonPath = null;',
    ' if ( process.platform == \'win32\' )',
    '  addonPath =\'h:/bin/\';',
    ' else',
    '  addonPath = \'~/bin/\';',
    ' xdhelcqPath = addonPath + \'xdhelcq.node\';',
    '} else {',
    ' xdhelcqPath = \'./xdhelcq.node\';',
    '}',
    '',
    'const xdhelcq =require( xdhelcqPath );',
    '// console.log( require(\'electron\').remote.process.argv );',
    'initialize( require(\'electron\').remote.process.argv );',
    '	</script>',
    '</body>',
  ].join("\n");

  console.log( "Av.");
  win.loadURL("data:text/html;charset=utf-8," + encodeURI(html), {baseURLForDataURL: "file://h:/hg/epeios/tools/xdhq/examples/common/blank/"});
//  win.loadURL("data:text/html;charset=utf-8," + encodeURI(html));
console.log( "Ap.");

  // Open the DevTools.
  win.webContents.openDevTools()

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
