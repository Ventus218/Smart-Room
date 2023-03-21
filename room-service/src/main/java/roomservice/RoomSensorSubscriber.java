package roomservice;

public interface RoomSensorSubscriber {
    void newSensorStateAvailable(SensorBoardMessage message);
}
