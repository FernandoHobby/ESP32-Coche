let anteriorAccion = 0;
let anteriorIndicepwm = 5;
const valorespwm = [75,90,105,120,135,150,165,180,195,210,225,240,255];

let gateway = 'ws://192.168.4.1/ws';

let websocket;

window.addEventListener('load', onLoad);

function onLoad(event) {
	initWebSocket();
}

function initWebSocket() {
	console.log("WebSocket -> "+ gateway);
	websocket = new WebSocket(gateway);
	websocket.onopen    = onOpen;
	websocket.onclose   = onClose;
	websocket.onmessage = onMessage;
}

function onOpen(event) {
	console.log("WebSocket -> Conectado");
	document.getElementById("conexion").innerHTML="Conectado";
	document.getElementById("conexion").style.color="green";	
	barra(5);
}

function onClose(event) {
	console.log("WebSocket -> NO conectado");
	document.getElementById("conexion").innerHTML="NO Conectado";
	document.getElementById("conexion").style.color="red";
}

function onMessage(event) {
	const recibido = event.data;
	console.log(recibido);
	const jsonDoc = JSON.parse(recibido);
	document.getElementById("voltaje").innerHTML=  jsonDoc["voltaje"].toFixed(1);
	document.getElementById("corriente").innerHTML= jsonDoc["corriente"].toFixed(0);
	document.getElementById("capacidad").innerHTML= jsonDoc["capacidad"].toFixed(0);
	document.getElementById("parar").innerHTML= jsonDoc["parar"];
}

function pulsa(accion) {
	enviarCoche(accion, valorespwm[anteriorIndicepwm]);
	anteriorAccion = accion;
}

function barra(indicepwm) {
	document.getElementById("pwm").innerHTML="pwm = " + valorespwm[indicepwm];
	enviarCoche(anteriorAccion,valorespwm[indicepwm]);
	anteriorIndicepwm = indicepwm;
}

function enviarCoche(accion, pwm){
	let miObjeto = new Object();
	miObjeto.accion = Number(accion);
	miObjeto.pwm = Number(pwm);
	let enviarCoche = JSON.stringify(miObjeto);
	console.log("WebSocket control -> " + enviarCoche);
	websocket.send(enviarCoche);
}