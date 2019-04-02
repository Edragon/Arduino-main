// Program: wifi-carAP.js  2015-11-16
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
//screen.lockOrientation("portrait");
var lastMove = 0;
var version = 8;
document.getElementById("vector").innerHTML ="vector1";
function move_car(left, right) {
  var now = Date.now();
  if (lastMove + 200 < now) {  // orig. 200 ms
     lastMove = now; 
     var request = new XMLHttpRequest();
     // if direction is opposite, change sign of +left and +right
     request.open('GET', '/engines?left=' + Math.round(-left) + "&right=" + Math.round(-right), true);
     request.send(null);
  }
}

function move(dir) {
    //var e = event.keyCode;
    if (dir=='f'){ move_car(-1000, -1000);}
    if (dir=='b'){ move_car(1000, 1000);}
    if (dir=='l'){ move_car(-1000, 1000);}
    if (dir=='r'){ move_car(1000, -1000);}
}
if (window.DeviceMotionEvent) {
  window.addEventListener('devicemotion', deviceMotionHandler, false);
  document.getElementById("dmEvent").innerHTML = "Accelerometer OK";
} else {
  document.getElementById("dmEvent").innerHTML = "Accelerometer not supported.";
}

function deviceMotionHandler(eventData) {
  //document.getElementById("vector").innerHTML ="vector2";
  acceleration = eventData.accelerationIncludingGravity;
  var left = 0;
  var right = 0;
  if (Math.abs(acceleration.y) > 1) { // back-/forward
    var speed = acceleration.y * 100;
    if (acceleration.y > 0) { // add 300 to decrease dead zone
        left = Math.min(1023, speed + acceleration.x * 40 + 300);
        right = Math.min(1023, speed - acceleration.x * 40 + 300);
    } else {
        left = Math.max(-1023, speed + acceleration.x * 40 - 300);
        right = Math.max(-1023, speed - acceleration.x * 40 - 300);       
    }
  } else if (Math.abs(acceleration.x) > 1) { // circle only
    var speed = Math.min(1023, Math.abs(acceleration.x) * 100);
    if (acceleration.x > 0) {
      left = Math.min(1023, speed + 300);
      right = Math.max(-1023, -speed - 300); 
    } else {
      left = Math.max(-1023, -speed - 300);  
      right = Math.min(1023, speed + 300);
    }
  }
  if (Math.abs(left) > 200 || Math.abs(right) > 200) { // orig. 100,100
    move_car(left, right);
  }
  var direction = "stop";
  // if direction is opposite, change sign of +left and +right
  var acc_x = Math.round(acceleration.x);
  var acc_y = Math.round(acceleration.y);
  var acc_z = Math.round(acceleration.z);
  var leftD = Math.round(-left);
  var rightD = Math.round(-right);

  direction = "[" + acc_x + "," + acc_y + "," + acc_z  + "]<BR/>" + leftD + ", " + rightD + "<BR/>version: " + version; 
  document.getElementById("vector").innerHTML =direction;
  //document.getElementById("block").style.fontSize = "x-large";
  //getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
}
