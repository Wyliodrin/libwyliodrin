"use strict"

var _=require('underscore');
var redis = require("redis");

var IP = '127.0.0.1'
var CHANNEL_SERVER = "communication_server:";
var CHANNEL_CLIENT = "communication_client:";

var port; 
var client;
var channelClient = {};

//port-ul il iau din env daca nu e definit
function initCommunication(redis_port)
{
	port = redis_port;
	try
	{
		client = redis.createClient(port, IP, {});
		return 0;
	}
	catch (e)
	{
		console.log("Redis cannot connect "+e);
		return -1;
	}
}

function openConnection(label, handlerFunction)
{
	try
	{
		var chClient = redis.createClient(port, IP, {});
		channelClient[label] = chClient;
		
		chClient.subscribe(CHANNEL_CLIENT+label);

		chClient.on("message", function(channel, message){
			var mes = JSON.parse(message);
			handlerFunction(mes['from'], 0, mes['data']);
		});
	}
	catch (e)
	{
		console.log("Redis cannot connect "+e);
	}
}

function sendMessage(wyliodrin_id, label, data)
{
	var message = {id:wyliodrin_id, data:data};
	client.publish(CHANNEL_SERVER+label, JSON.stringify(message));
}

function closeConnection(label)
{
	if (channelClient[label])
	{
		channelClient[label]. quit();
		channelClient[label] = null;
	}		
}

function closeCommunication ()
{
	client.quit();
	_.each(channelClient, function(value, key,list){
		value.quit();
	})
}

exports.initCommunication = initCommunication;
exports.openConnection = openConnection;
exports.sendMessage = sendMessage;
exports.closeConnection = closeConnection;
exports.closeCommunication = closeCommunication;
