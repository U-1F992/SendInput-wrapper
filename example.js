var oWshShell = new ActiveXObject("WScript.Shell");

WScript.StdOut.Write("");

radius = 0;
while(true) {
    var x = 500 + Math.floor(Math.sin(radius) * 100);
    var y = 500 + Math.floor(Math.cos(radius) * 100);
    
    oWshShell.Run('rundll32 SendInput.dll,_SendInput {"type":0,"mi":{"dx":' + x + ',"dy":' + y + ',"dwFlags":32769}}');
    WScript.StdOut.Write("\r" + x + "," + y);

    radius = radius < 2 * Math.PI ? radius + 0.1 : 0;
}