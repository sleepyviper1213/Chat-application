const input = document.querySelector('#textarea');
const messages = document.querySelector('#messages');
const username = document.querySelector('#username');
const send = document.querySelector('#send');

const url = 'wss://localhost:8080';
const ws = new WebSocket(url);

ws.addEventListener('message', message => {
	insertMessage(JSON.parse(message.data));
});

ws.addEventListener('close', event => {
	console.log('Client notified socket has closed', event);
});

send.addEventListener('click', () => {
	const message = {
		username: username.value,
		content: input.value,
	};

	ws.send(JSON.stringify(message));
	input.value = '';
});

/**
 * Insert a message into the UI
 * @param {Message that will be displayed in the UI} messageObj
 */
function insertMessage(messageObject) {
	// Create a div object which will hold the message
	const message = document.createElement('div');

	// Set the attribute of the message div
	message.setAttribute('class', 'chat-message');
	console.log('name: ' + messageObject.username + ' content: ' + messageObject.content);
	message.textContent = `${messageObject.username}: ${messageObject.content}`;

	// Append the message to our chat div
	messages.append(message);

	// Insert the message as the last message of our chat
	messages.insertBefore(message, messages.nextSibling);
}
