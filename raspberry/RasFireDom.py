#Para ello nececitamos tener instalado mosquitto y mosquitto_clients
#Ademas de instalar el cliente para python que en este caso utilizamos paho

import paho.mqtt.client as mqtt
import time
#from firebase_admin import db
import pyrebase
firebaseConfig = {
    "Colocamos la configuracion de la firebase"
}
firebase = pyrebase.initialize_app(firebaseConfig);
db = firebase.database()

client = mqtt.Client("Raspberry")

def stream_handler(message):
        #print(message["event"])
        #print(message["path"])
        #print(message["data"])
        if (message['path'] == "/foco1"):
                focoSala(message)

        if (message['path'] == '/foco3'):
                lampSala(message)

def lampSala(message):
        if (message['data'] == {u'status': True}):
                client.publish("casa/sala/lamparaSala", "on")
        else:
                client.publish('casa/sala/lamparaSala', 'off')

def focoSala(message):
        if (message['data'] == {u'status': True}):
                client.publish("casa/sala/focoSala", "on")
        else:
                client.publish('casa/sala/focoSala', 'off')

#Conectarse al broker
def on_connect(client, userdata, flags, rc):
        print('Connected (%s)' %client._client_id)
        client.subscribe(topic='casa/sala/focoSala')
        client.subscribe(topic='casa/sala/lamparaSala')
        client.subscribe(topic='casa/sala/')

def on_message(client, userdata, message):
        #print('-----------')
        #print('topic: %s' %message.topic)
        db.child('casa/')
        if (message.topic == 'casa/sala/focoSala'):
                db.child('foco1/')
                if (str(message.payload.decode("utf-8")) == 'on'):
                        db.update({"status":True})
                else:
                        db.update({'status':False})

        if (message.topic == 'casa/sala/lamparaSala'):
                db.child('foco3/')
                if (str(message.payload.decode("utf-8")) == 'on'):
                        db.update({'status':True})
                else:
                        db.update({'status':False})

        if (message.topic == 'casa/sala/'):
                if (str(message.payload.decode("utf-8")) == 'on'):
                        db.child('foco1')
                        db.update({'status':True})
                        db.child('casa/foco3')
                        db.update({'status':True})
                else:
                        db.child("foco1")
                        db.update({'status':False})
                        db.child('casa/foco3')
                        db.update({'status':False})

def main():
        #client = mqtt.Client("Raspberry")
        client.on_connect = on_connect
        client.on_message = on_message
        client.connect(host="192.168.100.10", port="1883")
        client.loop_forever()
        #client.loop_start()
        #my_stream = db.child("casa/").stream(stream_handler)

my_stream = db.child('casa/').stream(stream_handler)

if __name__ == '__main__':
        main()

my_stream.close()
#client.lopp_stop()
client.exit()
sys.exit(0)