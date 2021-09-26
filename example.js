var global = Function('return this')();
if (!global.JSON) {
    var html = new ActiveXObject('htmlfile');
    html.write('<meta http-equiv="x-ua-compatible" content="IE=11" />');
    global.JSON = html.parentWindow.JSON;
    html.close();
}

var oWshShell = new ActiveXObject("WScript.Shell");

var MOUSEEVENTF_MOVE = 0x0001;
var MOUSEEVENTF_ABSOLUTE = 0x8000;

var radius = 0;

WScript.StdOut.Write("");
while(true) {
    var x = 500 + Math.floor(Math.sin(radius) * 100);
    var y = 500 + Math.floor(Math.cos(radius) * 100);

    var input = {
        "type": 0,
        "mi": {
            "dx": x,
            "dy": y,
            "dwFlags": MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE
        }
    };
    oWshShell.Run('rundll32 SendInput.dll,_SendInput ' + JSON.stringify(input));
    WScript.StdOut.Write("\r" + x + "," + y);

    radius = radius < 2 * Math.PI ? radius + 0.1 : 0;
}
