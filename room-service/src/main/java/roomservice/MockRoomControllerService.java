package roomservice;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class MockRoomControllerService extends Thread implements RoomControllerService {

    private List<RoomControllerCommunicationSubscriber> subscribers = new ArrayList<>();
    private Optional<String> sendMessage;
    private final int messageFrequence;

    public MockRoomControllerService(int messageFrequenceMillis) {
        messageFrequence = messageFrequenceMillis;
        sendMessage = Optional.empty();
    }

    synchronized public Optional<String> getSendMessage() {
        return sendMessage;
    }

    synchronized public void setSendMessage(Optional<String> sendMessage) {
        this.sendMessage = sendMessage;
    }

    public void subscribe(RoomControllerCommunicationSubscriber sub) {
        this.subscribers.add(sub);
    }

    @Override
    public void run() {
        super.run();

        try {
            sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        while (true) {
            String msg = "SERVICE";
            // String msg = "BLUETOOTH;true;75";
            RoomControllerMessage controllerMessage = parseMessage(msg);
            notifySubscribers(controllerMessage);
            log(msg);

            Optional<String> sendMessage = getSendMessage();
            if (sendMessage.isPresent()) {
                setSendMessage(Optional.empty());
            }

            try {
                sleep(messageFrequence);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private RoomControllerMessage parseMessage(String s) {
        String[] components = s.split(";");
        if(components[0].toLowerCase().equals("service")) {
            return RoomControllerMessage.controllerServiceInputState();
        } else {
            return RoomControllerMessage.controllerBluetoothInputState(Boolean.parseBoolean(components[1]), Integer.parseInt(components[2]));
        }
    }

    private void log(String msg) {
        System.out.println("[ROOM-CONTROLLER] "+msg);
    }

    private void notifySubscribers(RoomControllerMessage message) {
        for (RoomControllerCommunicationSubscriber sub : subscribers) {
            sub.newControllerMessageAvailable(message);
        }
    }
}
