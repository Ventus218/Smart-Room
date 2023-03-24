package roomservice;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.Json;
import io.vertx.mqtt.MqttClient;

import java.util.ArrayList;
import java.util.List;

public class MQTTAgent extends AbstractVerticle implements RoomSensorService {

    private List<RoomSensorSubscriber> subscribers = new ArrayList();

    @Override
    public void start() {
        MqttClient client = MqttClient.create(vertx);

        client.connect(1883, "broker.mqtt-dashboard.com", c -> {

            log("connected");

            log("subscribing...");
            client.publishHandler(s -> {
                SensorBoardMessage message = Json.decodeValue(s.payload().toString(), SensorBoardMessage.class);
                log("MQTT message received: " +  message.toString());
                notifySubscribers(message);

            })
            .subscribe("esiot-2022/aleventu/sensor-board", 0);
        });
    }

    private void log(String msg) {
        System.out.println("[ROOM-SENSOR] "+msg);
    }

    public synchronized void subscribe(RoomSensorSubscriber sub) {
        this.subscribers.add(sub);
    }

    private void notifySubscribers(SensorBoardMessage message) {
        for (RoomSensorSubscriber sub : subscribers) {
            sub.newSensorStateAvailable(message);
        }
    }
}