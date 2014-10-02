import redis
import os
import json

port = 6379
channelClient = {}
threads = {}
handlers = {}
CHANNEL = "communication"
client = None

def initCommunication (redis_port):
	global port
	global client
	if redis_port is None:
		redis_port = os.getenv("wyliodrin_port", 6379);
	client = redis.StrictRedis(host='localhost', port=redis_port, db=0)
	port = redis_port
	return 0

def myHandlerFunction (message):
	global handlers
	global CHANNEL

	channel = message['channel']
	label = int(channel[len(CHANNEL):])
	myHandler = handlers[label]

	mes = json.loads(message['data'])
	print mes
	fromId = mes['from']
	data = mes['data']

	myHandler(fromId, 0, data)

def openConnection (label, handlerFunction):
	global channelClient
	global threads
	global handlers
	global CHANNEL

	handlers[label] = handlerFunction
	r = redis.StrictRedis(host='localhost', port=port, db=0)
	p = r.pubsub(ignore_subscribe_messages=True)
	p.subscribe(**{CHANNEL+str(label): myHandlerFunction})
	# keyword = CHANNEL+str(label)
	#p.subscribe(channel1=handlerFunction)
	channelClient[label] = p
	thread = p.run_in_thread()
	threads[label] = thread

def sendMessage (wyliodrin_id, label, data):
	global client

	message = {"id":wyliodrin_id, "data":data}
	client.publish(CHANNEL+str(label), json.dumps(message))

def closeConnection(label):
	global threads
	global channelClient

	thread = threads[label]
	if thread is not None:
		thread.stop()
		del threads[label]
	p = channelClient[label]
	if p is not None:
		p.close()
		del channelClient[label]

def closeCommunication():
	global client
	global threads
	global channelClient

	client.close()

	for thread in threads:
		thread.stop()
	for c in channelClient:
		c.close()

def func(wfrom, err, data):
	print wfrom, data

openConnection (2, func)
closeConnection(2)
# sendMessage("ioana", 2, "testam")
