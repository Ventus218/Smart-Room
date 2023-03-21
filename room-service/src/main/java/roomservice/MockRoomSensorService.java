package roomservice;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MockRoomSensorService extends Thread implements RoomSensorService {

    private List<RoomSensorSubscriber> subscribers = new ArrayList();
    private final int messageFrequence;

    public MockRoomSensorService(int messageFrequenceMillis) {
        this.messageFrequence = messageFrequenceMillis;
    }

    @Override
    public void run() {
        super.run();
        while (true) {
            Random r = new Random();
            boolean motion = r.nextBoolean();
            int lightLevel = r.nextInt(500, 3000);
            notifySubscribers(new SensorBoardMessage(motion, lightLevel));

            try {
                sleep(messageFrequence);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private void log(String msg) {
        System.out.println("[ROOM-SENSOR] "+msg);
    }

    @Override
    public synchronized void subscribe(RoomSensorSubscriber sub) {
        this.subscribers.add(sub);
    }

    private void notifySubscribers(SensorBoardMessage message) {
        log(message.toString());
        for (RoomSensorSubscriber sub : subscribers) {
            sub.newSensorStateAvailable(message);
        }
    }
}
