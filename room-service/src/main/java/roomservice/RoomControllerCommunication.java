package roomservice;

import serialcommunication.CommChannel;
import serialcommunication.ExtendedSerialCommChannel;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class RoomControllerCommunication extends Thread implements RoomControllerService {

    private List<RoomControllerCommunicationSubscriber> subscribers;
    private String port;
    private int rate;
    private CommChannel channel;
    private Optional<String> sendMessage;

    public RoomControllerCommunication(String port, int rate) {
        this.subscribers = new ArrayList<RoomControllerCommunicationSubscriber>();
        this.port = port;
        this.rate = rate;
        this.channel = null;
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

    private void init() throws Exception {
        this.channel = new ExtendedSerialCommChannel(port, rate);

        /* attesa necessaria per fare in modo che Arduino completi il reboot */
        log("Waiting for boot...");
        sleep(5000);
        log("Ready.");
    }

    @Override
    public void run() {
        super.run();
        try {
            init();

            while (true) {
                if (channel.isMsgAvailable()) {
                    String msg = channel.receiveMsg();
                    try {
                        RoomControllerMessage controllerMessage = parseMessage(msg);
                        notifySubscribers(controllerMessage);
                    } catch (Exception e) {
                        log(msg);
                    }
                }

                Optional<String> sendMessage = getSendMessage();
                if (sendMessage.isPresent()) {
                    channel.sendMsg(sendMessage.get());
                    setSendMessage(Optional.empty());
                }
                // continue from here
            }
        } catch (Exception e) {
            System.out.println("Exception: " + e.toString());
            System.exit(1);
        }
    }

    private void log(String msg) {
        System.out.println("[ROOM-CONTROLLER] "+msg);
    }

    private RoomControllerMessage parseMessage(String s) throws Exception {
        String[] components = s.split(";");
        if(components[0].toLowerCase().equals("service")) {
            return RoomControllerMessage.controllerServiceInputState();
        } else if(components[0].toLowerCase().equals("bluetooth")) {
            return RoomControllerMessage.controllerBluetoothInputState(Boolean.parseBoolean(components[1]), Integer.parseInt(components[2]));
        } else {
            throw new IllegalArgumentException("Wrong RoomControllerInputState format");
        }
    }

    private void notifySubscribers(RoomControllerMessage message) {
        for (RoomControllerCommunicationSubscriber sub : subscribers) {
            sub.newControllerMessageAvailable(message);
        }
    }
}
