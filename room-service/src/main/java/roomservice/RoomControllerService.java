package roomservice;

import java.util.Optional;

public interface RoomControllerService {
    public void subscribe(RoomControllerCommunicationSubscriber sub);

    public Optional<String> getSendMessage();

    public void setSendMessage(Optional<String> sendMessage);
}
