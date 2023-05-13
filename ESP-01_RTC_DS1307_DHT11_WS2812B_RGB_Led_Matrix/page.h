const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>Great Projects</title>
        <script>
        var Message = 0;
        var data_val1 = 0;
        var data_val2 = 0;
        var tz_val;
      function DisplayCurrentTime() {
          var dt = new Date();
  var weekday = new Array(7);
  weekday[0] = "Sunday";
  weekday[1] = "Monday";
  weekday[2] = "Tuesday";
  weekday[3] = "Wednesday";
  weekday[4] = "Thursday";
  weekday[5] = "Friday";
  weekday[6] = "Saturday";
  var dow = weekday[dt.getDay()];
document.getElementById("datetime").innerHTML = (dow) +" "+ (dt.toLocaleString());
        setTimeout('DisplayCurrentTime()', 1000);
      }
function myFunction() {
  var d = new Date();
  var n = d.getTime()/1000;
  var request = new XMLHttpRequest();
  var strLine = "localdatime?locadt=" + n;
  request.open("GET", strLine, false);
  request.send(null);
}
function myFunctionT() {
  var request = new XMLHttpRequest();
  var nt = document.getElementById("mytmie");
  var nd = document.getElementById("myddate");
  var strLine = "restime?htmie=" + nt.value;
      strLine += "&ddate=" + nd.value;
  request.open("GET", strLine, false);
  request.send(null);
}
function myFunctionTz() {
  var nz = document.getElementById("mylocatz");
  var request = new XMLHttpRequest();
  var strLine = "mytimezon?locatz=" + nz.value;
  request.open("GET", strLine, false);
  request.send(null);
}
function myFunctionN() {
  var request = new XMLHttpRequest();
  request.open("GET", "ntptime", false);
  request.send(null);
}
function myFunctionS() {
  var request = new XMLHttpRequest();
  request.open("GET", "savetime", false);
  request.send(null);
}
        function GetArduinoInputs()
        {
          var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
            data_val1  = this.responseXML.getElementsByTagName('analog')[0].childNodes[0].nodeValue;
            data_val2 = this.responseXML.getElementsByTagName('analog')[1].childNodes[0].nodeValue;
            tz_val = this.responseXML.getElementsByTagName('analog')[2].childNodes[0].nodeValue;
            Message = this.responseXML.getElementsByTagName('analog')[3].childNodes[0].nodeValue;
    }
    if(tz_val < 0) { document.getElementById("TZmesg").innerHTML = "GMT" + tz_val; }
    else { document.getElementById("TZmesg").innerHTML = "GMT+" + tz_val; }

    if(Message == 0) {  
                  var codeBlock1 = '<button type="submit" class="button">Mode Clock</button>';
                  document.getElementById("modemesg").innerHTML = codeBlock1;
    }
    else  { 
                  var codeBlock1 = '<button type="submit" class="button">Mode Message</button>';
                  document.getElementById("modemesg").innerHTML = codeBlock1;
    }

                  var codeBlock2 = '<h1>' + data_val1 + ' &degC</h1>';
                  var codeBlock3 = '<h1>' + data_val2 + ' %RH</h1>';
document.getElementById("Temp").innerHTML = codeBlock2;
document.getElementById("Humi").innerHTML = codeBlock3;
    var elem = document.getElementById("Humi");
    elem.style.color = "#0000ff";
    elem.style.fontSize = "30px";
    var elem = document.getElementById("Temp");
    elem.style.color = "#ff0000";
    elem.style.fontSize = "30px";
  }
  xhttp.open("GET", "readtemhu", true);
  xhttp.send();
            setTimeout('GetArduinoInputs()', 5000);
        }
      document.addEventListener('DOMContentLoaded', function() {
        DisplayCurrentTime(),GetArduinoInputs();
      }, false);
    </script>
      <style>

  body {
  text-align: center;
  background-color: #ffaaaa;
  }
/* Full-width input fields */
input[type=text], input[type=password] {
  width: 100%;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  box-sizing: border-box;
}
.container {
  padding: 16px;
}

span.psw {
  float: right;
  padding-top: 16px;
}

/* The Modal (background) */
.modal {
  display: none; /* Hidden by default */
  position: fixed; /* Stay in place */
  z-index: 1; /* Sit on top */
  left: 0;
  top: 0;
  width: 100%; /* Full width */
  height: 100%; /* Full height */
  overflow: auto; /* Enable scroll if needed */
  background-color: rgb(0,0,0); /* Fallback color */
  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */
  padding-top: 60px;
}

/* Set a style for all buttons */
button {
  padding: 5px 5px 5px 5px;
  text-shadow: 2px 2px #000000;
  width: 100%;
  border: #fbfb00 solid 3px;
  background-color: #0000ff;
  color:white;
  font-size:22px;
  padding-bottom:5px;
  font-weight:600;
  -moz-border-radius: 10px;
  -webkit-border-radius: 10px;
}

button:hover {
  text-shadow: 2px 2px #ff0000;
  opacity: 0.8;
}

/* Modal Content/Box */
.modal-content {
  background-color: #fefefe;
  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
  border: 1px solid #888;
  width: 40%; /* Could be more or less, depending on screen size */
}
/* Add Zoom Animation */
.animate {
  -webkit-animation: animatezoom 0.6s;
  animation: animatezoom 0.6s
}

@-webkit-keyframes animatezoom {
  from {-webkit-transform: scale(0)} 
  to {-webkit-transform: scale(1)}
}
  
@keyframes animatezoom {
  from {transform: scale(0)} 
  to {transform: scale(1)}
}

/* Change styles for span and cancel button on extra small screens */
@media screen and (max-width: 300px) {
  span.psw {
     display: block;
     float: none;
  }
  .cancelbtn {
     width: 100%;
  }
}
.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 15px;
  background: #ffff00;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  border: 2px solid #ff00ff;
  opacity: 1;
}


.slider::-moz-range-thumb {
  width: 15px;
  height: 25px;
  background: #FF0F50;
  cursor: pointer;
}
  .data-input {
  text-align: center;
  background-color: #bbbbff;
  font-size:26px;
  color:red;
  border: 5px solid #444444;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  table {
  text-align: center;
  border: 2px solid #ff00ff;
  background-color: #ffffff;
  width:100%;
  color: #0000ff;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  td {
  border: #fb0000 solid 2px;
  background-color: #111111;
  padding: 5px;
  -moz-border-radius: 12px;
  -webkit-border-radius: 12px;
  }
  .dtime {
  padding: 5px 5px 5px 5px;
  width: 100%;
  color:#ffffff;
  font-size:30px;
  padding-bottom:5px;
  font-weight:700;
  }
  .button-blue {
  padding: 5px 5px 5px 5px;
  width: 100%;
  border: #fbfb00 solid 3px;
  background-color: #4444ff;
  color:white;
  font-size:22px;
  padding-bottom:5px;
  font-weight:700;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  .button-blue:hover {
  opacity: 1;
  text-shadow: 2px 2px #ff0000;
  color: #ff93bd;
  }
  </style>
    </head>
    <body>
    <table>
   <tr>
   <td style="width:16%">
    <b  class="button-blue"v><span id="TZmesg"></span></b>
   </td>
   <td style="width:24%">
  <form action="/" method="POST">
   <input type="number" value="0" name="locatz" id="mylocatz" min="-12" max="+12" class="button-blue" style="width:22%">
   <button onclick="myFunctionTz()" class="button" style="width:68%">Set</button>
  </form>
   </td>
   <td style="width:38%">
   <b class="dtime"><span id="datetime"></span></b></td>
   <td style="width:28%">
  <form action="/" method="POST">
   <button onclick="myFunctionN()" class="button">NTP Time</button>
  </form></td>
  </tr>
    </table>
    <table>
   <tr>
   <td style="width:14%">
  <form action="/MesColor" method="POST">
   <input type="number" value="5" name="Color0" min="0" max="10" class="button-blue" style="width:26%">
   <input type="submit" value="Color" class="button-blue" style="width:50%">
  </form></td>
   <td style="width:62%">
  <form action="/" method="POST">
   <input type="date" value="0" name="ddate" id="myddate" class="button-blue" style="width:32%">
   <input type="time" value="00:00:00" name="htmie" id="mytmie" class="button-blue" step="2" style="width:32%">
   <button onclick="myFunctionT()" class="button" style="width:22%">Set</button>
  </form></td>
   <td style="width:22%">
  <form action="/" method="POST">
   <button onclick="myFunction()" class="button">Set local Time</button>
  </form></td>
  </tr>
    </table>
    <table>
   <tr>
  <form action="/myMesg" method="POST">
   <td><div class="slidecontainer">
  <input type="text" placeholder="Enter your message here..."  name="Message" maxlength="655" style="width:78%;height:35px;font-size:30px">
  <input type="submit" value="Send" class="button-blue" style="width:20%">
  </div></td></form>
  </tr>
   <tr>
<form action="/MesSpeed0" method="POST">
   <td>
  <b style="width:8%"><span class="button-blue" id="speed0"></span></b>
  <input type="range" name="Speed0" min="5" max="300" value="30" class="slider" style="width:74%" id="mySpeed0">
  <input type="submit" value="Speed Mesg" style="width:18%" class="button-blue">
  </td>
  <script>
var slider0 = document.getElementById("mySpeed0");
var output0 = document.getElementById("speed0");
output0.innerHTML = slider0.value;

slider0.oninput = function() {
  output0.innerHTML = this.value;
}
</script>
</form>
  </tr>
   <tr>
<form action="/MesSpeed1" method="POST">
   <td>
  <b style="width:8%"><span class="button-blue" id="speed1"></span></b>
  <input type="range" name="Speed1" min="1" max="500" value="50" class="slider" style="width:74%" id="mySpeed1">
  <input type="submit" value="Speed Frame" style="width:18%" class="button-blue">
  </td>
  <script>
var slider1 = document.getElementById("mySpeed1");
var output1 = document.getElementById("speed1");
output1.innerHTML = slider1.value;

slider1.oninput = function() {
  output1.innerHTML = this.value;
}
</script>
</form>
  </tr>
   <tr>
<form action="/MesBright" method="POST">
   <td>
  <b style="width:8%"><span class="button-blue" id="bright"></span></b>
  <input type="range" name="Bright" min="0" max="250" value="25" class="slider" style="width:74%" id="myBright">
  <input type="submit" value="setBrightness" style="width:18%" class="button-blue">
  </td>
  <script>
var sliderb = document.getElementById("myBright");
var outputb = document.getElementById("bright");
outputb.innerHTML = sliderb.value;

sliderb.oninput = function() {
outputb.innerHTML = this.value;
}
</script>
</form>
  </tr>
   <tr>
<form action="/MesSpeed4" method="POST">
   <td>
  <b style="width:8%"><span class="button-blue" id="speed4"></span></b>
  <input type="range" name="Speed4" min="1" max="9" value="5" class="slider" style="width:74%" id="mySpeed4">
  <input type="submit" value="Set" style="width:18%" class="button-blue">
  </td>
  <script>
var slider4 = document.getElementById("mySpeed4");
var output4 = document.getElementById("speed4");
output4.innerHTML = slider4.value;

slider4.oninput = function() {
  output4.innerHTML = this.value;
}
</script>
</form>
  </tr>
   <tr>
<form action="/MesSpeed5" method="POST">
   <td>
  <b style="width:8%"><span class="button-blue" id="speed5"></span></b>
  <input type="range" name="Speed5" min="1" max="9" value="6" class="slider" style="width:74%" id="mySpeed5">
  <input type="submit" value="Set" style="width:18%" class="button-blue">
  </td>
  <script>
var slider5 = document.getElementById("mySpeed5");
var output5 = document.getElementById("speed5");
output5.innerHTML = slider5.value;

slider5.oninput = function() {
  output5.innerHTML = this.value;
}
</script>
</form>
  </tr>
    </table>
    <table>
   <tr>
   <td style="width:48%">
    <b><span id="Temp"></span></b>
   </td>
   <td style="width:48%">
   <b><span id="Humi"></span></b>
   </td>
  </tr>
    </table>
    <table>
   <tr>
        <td style='width:25%'>
  <form action="/" method="POST">
   <button onclick="myFunctionS()" class="button">Save</button>
  </form>
   </td>
        <td style='width:25%'>
    <div class="slidecontainer">
    <button onclick="document.getElementById('id01').style.display='block'" class="button">Connect to Wi-Fi</button>
  </div>
    <div id="id01" class="modal">
  <form class="modal-content animate" action="/Mywifi" method="POST">
    <div  class="data-input">
    <div class="slidecontainer">
      <label for="usname"><b>Wifi Ssid</b></label>
      <input type="text" placeholder="Enter Ssid" name="usname" required>
      <label for="pssw"><b>Password</b></label>
      <input type="password" placeholder="Enter Password" name="pssw">
      <button type="submit" class="button"><b>Connect</b></button>
    </div>
    </div>
  </form>
</div>

<script>
// Get the modal
var modal = document.getElementById('id01');

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}
</script></td>
        <td style='width:25%'>
  <form action="/MesMode" method="POST">
    <b><span id="modemesg"></span></b>
  </form>
   </td>
        <td style='width:25%'>
  <form action="/restesp" method="POST">
      <button type="submit" name = "rerset" class="button">Restart ESP</button>
  </form>
   </td>
  </tr>
    </table>
    </body>
</html>
)=====";
