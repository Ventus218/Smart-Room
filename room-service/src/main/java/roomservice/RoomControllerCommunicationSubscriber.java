package roomservice;

public interface RoomControllerCommunicationSubscriber {
    void newControllerMessageAvailable(RoomControllerMessage message);
}
