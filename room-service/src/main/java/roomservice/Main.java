package roomservice;

import io.vertx.core.Vertx;

public class Main {

    public static void main(String[] args) throws Exception {

        RoomControllerCommunication roomControllerService = new RoomControllerCommunication("/dev/cu.usbmodem143101", 9600);
        //MockRoomControllerService roomControllerService = new MockRoomControllerService(2000);

        RoomService roomService = new RoomService(roomControllerService, SmartRoomControlState.AUTOMATIC);

        Vertx vertx = Vertx.vertx();

        MQTTAgent roomSensorService = new MQTTAgent();
        //MockRoomSensorService roomSensorService = new MockRoomSensorService(2000);
        roomSensorService.subscribe(roomService);

        HTTPServer server = new HTTPServer(roomService, "/Users/Alessandro/Desktop/Smart-Room/room-dashboard", 8081);

        roomControllerService.subscribe(roomService);

        roomControllerService.start();
        vertx.deployVerticle(roomSensorService);
        //roomSensorService.start();
        vertx.deployVerticle(server);

        Thread.sleep(5000);
        while (true) {
            Thread.sleep(500);
            roomService.process();
        }
    }
}
