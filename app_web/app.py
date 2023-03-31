# #!/usr/bin/python

# import sys
# import paho.mqtt.client as mqtt

# def on_connect(mqttc, obj, flags, rc):
#     print("rc: "+str(rc))

# def on_message(mqttc, obj, msg):
#     print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))

# def on_publish(mqttc, obj, mid):
#     print("mid: "+str(mid))

# def on_subscribe(mqttc, obj, mid, granted_qos):
#     print("Subscribed: "+str(mid)+" "+str(granted_qos))

# def on_log(mqttc, obj, level, string):
#     print(string)

# mqttc = mqtt.Client(transport='websockets')   
# mqttc.on_message = on_message
# mqttc.on_connect = on_connect
# mqttc.on_publish = on_publish
# mqttc.on_subscribe = on_subscribe

# mqttc.connect("broker.mqttdashboard.com", 8000, 60)

# mqttc.subscribe("esp/test", 0)
# #mqttc.subscribe("$SYS/#", 0)

# mqttc.loop_forever()

""" flask_example.py

    Required packages:
    - flask
    - folium

    Usage:

    Start the flask server by running:

        $ python flask_example.py

    And then head to http://127.0.0.1:5000/ in your browser to see the map displayed

"""

from flask import Flask

import folium

app = Flask(__name__)


@app.route('/')
def index():
    start_coords = (46.9540700, 142.7360300)
    folium_map = folium.Map(location=start_coords, zoom_start=14)
    return folium_map._repr_html_()


if __name__ == '__main__':
    app.run(debug=True)