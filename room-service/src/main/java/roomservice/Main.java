package roomservice;

import io.vertx.core.Vertx;

public class Main {

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();

        MQTTAgent agent = new MQTTAgent();
        vertx.deployVerticle(agent);

        HTTPServer server = new HTTPServer();
        vertx.deployVerticle(server);
    }
}
