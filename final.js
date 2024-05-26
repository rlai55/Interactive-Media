let serial;
let systemState = "OFF"; 
let distance = 0;
let notificationSent = false; 

function setup() {
  serial = new p5.SerialPort();

  let options = { baudrate: 9600 }; 
  serial.openPort('COM7', options); 

  serial.on('connected', serverConnected);
  serial.on('open', portOpen);
  serial.on('data', serialEvent);
  serial.on('error', serialError);
  serial.on('close', portClose);
}

function draw() {
  
}

function serverConnected() {
  console.log('Connected to server.');
}

function portOpen() {
  console.log('The serial port opened.');
}

function serialEvent() {
  let currentString = serial.readLine();
  currentString = trim(currentString); 
  
  console.log("Received: " + currentString); 

  if (currentString === "ALERT" && !notificationSent) {
    sendIFTTTNotification();
    notificationSent = true; 
  }

  let data = currentString.split(",");
  if (data.length === 2) {
    systemState = data[0];
    distance = parseInt(data[1], 10); 

    // 重置通知标志的条件
    if (distance <= 10) {
      notificationSent = false;
    }
  }
}

function sendIFTTTNotification() {
  let url = `https://maker.ifttt.com/trigger/put_phone_down/with/key/b6qCSXPj38GzzySHXMOxWI`;

  fetch(url, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({ Value1: 'Put your phone down! You are doing an assignment now!!!' })
  })
  .then(response => {
    if (!response.ok) {
      throw new Error('Network response was not ok');
    }
    return response.json();
  })
  .then(data => {
    console.log('IFTTT notification sent', data);
  })
  .catch(error => {
    console.error('There was a problem with the fetch operation:', error);
  });
}

function serialError(err) {
  console.log('Something went wrong with the serial port. ' + err);
}

function portClose() {
  console.log('The serial port closed.');
}
