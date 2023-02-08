package roomservice;

import io.vertx.core.AbstractVerticle;
import io.vertx.mqtt.MqttClient;

public class MQTTAgent extends AbstractVerticle {

    public MQTTAgent() {
    }

    @Override
    public void start() {
        MqttClient client = MqttClient.create(vertx);

        client.connect(1883, "broker.mqtt-dashboard.com", c -> {

            log("connected");

            log("subscribing...");
            client.publishHandler(s -> {
//                System.out.println("There are new message in topic: " + s.topicName());
//                System.out.println("Content(as string) of the message: " + s.payload().toString());
//                System.out.println("QoS: " + s.qosLevel());
                System.out.println("sensor-board:\t" + s.payload().toString());
            })
            .subscribe("esiot-2022/aleventu/sensor-board", 2);
        });
    }


    private void log(String msg) {
        System.out.println("[MQTT AGENT] "+msg);
    }

}