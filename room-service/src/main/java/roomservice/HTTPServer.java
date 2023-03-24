package roomservice;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpMethod;
import io.vertx.core.json.Json;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.FileSystemAccess;
import io.vertx.ext.web.handler.StaticHandler;


public class HTTPServer extends AbstractVerticle {

    private RoomService roomService;
    private String roomDashboardPath;
    private int roomDashboardPort;

    public HTTPServer(RoomService roomService, String roomDashboardPath, int roomDashboardPort) {
        this.roomService = roomService;
        this.roomDashboardPath = roomDashboardPath;
        this.roomDashboardPort = roomDashboardPort;
    }

    @Override
    public void start() {
        // Create a Router
        Router router = Router.router(vertx);

        router.route(HttpMethod.GET, "/dashboard/*").handler(StaticHandler.create(FileSystemAccess.ROOT,roomDashboardPath));

        router.route(HttpMethod.GET, "/dashboard-data").handler(context -> {
            SensorBoardMessage sensorState = roomService.getSensorState();
            RoomControllerMessage lastControllerMessage = roomService.getLastControllerMessage();
            RoomControllerInput lastControllerInput = roomService.getLastControllerInput();

            JsonObject sensorData = new JsonObject()
                    .put("lightLevel", sensorState.getLightLevel())
                    .put("motion", sensorState.getMotion());

            JsonObject controllerData = null;

            if (lastControllerInput != null) {
                boolean lightOn;
                int rollerPercentage;

                if (lastControllerMessage == null || lastControllerMessage.getControllerInputState() == RoomControllerInputState.SERVICE) {
                    lightOn = lastControllerInput.isLightOn();
                    rollerPercentage = lastControllerInput.getRollerPercentage();
                } else {
                    lightOn = lastControllerMessage.isLightOn();
                    rollerPercentage = lastControllerMessage.getRollerPercentage();
                }
                controllerData = new JsonObject()
                        .put("controlState", roomService.getControlState())
                        .put("lightOn", lightOn)
                        .put("rollerPercentage", rollerPercentage);
            }

            JsonArray lightHistoryData = new JsonArray();

            for (LightHistoryEntry entry : roomService.getLightHistory()) {
                lightHistoryData.add(new JsonObject()
                        .put("time", entry.getTime().withNano(0).toString())
                        .put("lightOn", entry.getLightOn().isPresent() ? entry.getLightOn().get() : null));
            }

            context.json(new JsonObject()
                    .put("sensorData", sensorData)
                    .put("controllerData", controllerData)
                    .put("lightHistoryData", lightHistoryData));
        });

        router.route(HttpMethod.POST, "/controller").handler(BodyHandler.create()).handler(context -> {
            RoomDashboardMessage message = Json.decodeValue(context.getBodyAsString(), RoomDashboardMessage.class);
            try {
                roomService.setLastDashboardMessage(message);
            } catch (IllegalArgumentException e) {
                context.response().setStatusCode(409);
                context.response().setStatusMessage(e.getMessage());
            }
            context.response().setStatusCode(200);
            context.response().end();
        });

        // Create the HTTP server
        vertx.createHttpServer()
                // Handle every request using the router
                .requestHandler(router)
                // Start listening
                .listen(roomDashboardPort)
                // Print the port
                .onSuccess(server -> log("server started on port " + server.actualPort()));
    }


    private void log(String msg) {
        System.out.println("[HTTP Server] " + msg);
    }

}