package roomservice;

import java.time.LocalTime;
import java.util.*;

public class RoomService implements RoomControllerCommunicationSubscriber, RoomSensorSubscriber {

    private List<LightHistoryEntry> lightHistory = new ArrayList<>();
    private RoomControllerService roomControllerService;
    private SensorBoardMessage sensorState;
    private SmartRoomControlState controlState = SmartRoomControlState.AUTOMATIC;
    private RoomControllerInput lastControllerInput;
    private RoomControllerMessage lastControllerMessage;
    private RoomDashboardMessage lastDashboardMessage;

    private int lastRollerPercentage;

    public RoomService(RoomControllerService roomControllerService, SmartRoomControlState controlState) {
        this.roomControllerService = roomControllerService;
        this.controlState = controlState;
        this.sensorState = null;
        this.lastControllerInput = null;
        this.lastControllerMessage = null;
        this.lastRollerPercentage = 0;
        lightHistory.add(new LightHistoryEntry(Optional.empty(), LocalTime.of(0, 0, 0)));
    }

    synchronized public void process() {
        if (sensorState == null) { return; }

        LightHistoryEntry lastHistoryEntry = getLastHistoryEntry();
        if (LocalTime.now().isBefore(lastHistoryEntry.getTime())) { // need to empty out the entry list.
            Optional<Boolean> oldValue = lastHistoryEntry.getLightOn();
            lightHistory.clear();
            lightHistory.add(new LightHistoryEntry(oldValue, LocalTime.of(0, 0, 0)));
            lastHistoryEntry = getLastHistoryEntry();
        }

        // check if changing RoomControlState (using lastControllerMessage)
        if (lastControllerMessage == null || lastControllerMessage.getControllerInputState() == RoomControllerInputState.SERVICE) {
            RoomControllerInput input;

            // check if dashboard has requested control
            if (lastDashboardMessage != null && lastDashboardMessage.isDashboardControl()) {
                this.controlState = SmartRoomControlState.WEB_INTERFACE;
            } else {
                this.controlState = SmartRoomControlState.AUTOMATIC;
            }

            switch (controlState) {
                case AUTOMATIC:
                    input = computeRoomControllerInput();
                    break;
                case WEB_INTERFACE:
                    input = getRoomControllerInputFromWebInterface();
                    break;
                default:
                    throw new IllegalStateException("Unexpected value: " + controlState);
            }

            if (lastHistoryEntry.getLightOn().isEmpty() || input.isLightOn() != lastHistoryEntry.getLightOn().get()) {
                lightHistory.add(new LightHistoryEntry(Optional.of(input.isLightOn()), LocalTime.now()));
            }
            this.lastControllerInput = input;
            this.lastRollerPercentage = input.getRollerPercentage();
            roomControllerService.setSendMessage(Optional.of(encodeRoomControllerInput(input)));
        } else {
            if (controlState != SmartRoomControlState.BLUETOOTH) {
                controlState = SmartRoomControlState.BLUETOOTH;
                log("Switched control state to BLUETOOTH");
            }
            if (lastHistoryEntry.getLightOn().isEmpty() || lastControllerMessage.isLightOn() != lastHistoryEntry.getLightOn().get()) {
                lightHistory.add(new LightHistoryEntry(Optional.of(lastControllerMessage.isLightOn()), LocalTime.now()));
            }
            this.lastRollerPercentage = lastControllerMessage.getRollerPercentage();
        }
    }

    private LightHistoryEntry getLastHistoryEntry() {
        return lightHistory.get(lightHistory.size() - 1);
    }

    private void log(String msg) {
        System.out.println("[ROOM-SERVICE] "+msg);
    }

    private RoomControllerInput computeRoomControllerInput() {
        boolean lightOn = false;
        int rollerPercentage = lastRollerPercentage;
        boolean isDark = sensorState.getLightLevel() < 1500;

        if (sensorState.getMotion() && isDark) {
            lightOn = true;
        }

        if (LocalTime.now().getHour() > 8 && sensorState.getMotion()) {
            rollerPercentage = 100;
        }

        if (LocalTime.now().getHour() > 19 && !sensorState.getMotion()) {
            rollerPercentage = 0;
        }

        return new RoomControllerInput(lightOn, rollerPercentage);
    }

    private RoomControllerInput getRoomControllerInputFromWebInterface() {
        return new RoomControllerInput(lastDashboardMessage.isSetLightOn(), lastDashboardMessage.getSetRollerPercentage());
    }

    private String encodeRoomControllerInput(RoomControllerInput input) {
        // Message encoding: "inputChangeFlag;inputSource;lightOn;rollerPercentage" as "int;int;int;int"
        return "0;0;" + (input.isLightOn() ? "1" : "0") + ";" + input.getRollerPercentage();
    }

    private int map(int value, int inMin, int inMax, int outMin, int outMax) {
        return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    public SensorBoardMessage getSensorState() {
        return sensorState;
    }

    public RoomControllerInput getLastControllerInput() {
        return lastControllerInput;
    }

    public RoomControllerMessage getLastControllerMessage() {
        return lastControllerMessage;
    }

    public List<LightHistoryEntry> getLightHistory() {
        return new ArrayList<>(lightHistory);
    }

    public RoomDashboardMessage getLastDashboardMessage() {
        return lastDashboardMessage;
    }

    public SmartRoomControlState getControlState() {
        return controlState;
    }

    public void setLastDashboardMessage(RoomDashboardMessage lastDashboardMessage) throws IllegalArgumentException {
        if (lastDashboardMessage.isDashboardControl() && this.controlState == SmartRoomControlState.BLUETOOTH) {
            if (this.lastDashboardMessage == null || this.lastDashboardMessage.isDashboardControl() == false) {
                throw new IllegalArgumentException("Dashboard cannot take control while the controller is receiving Bluetooth input.");
            } else {
                throw new IllegalArgumentException("Dashboard have lost control because the controller started receiving Bluetooth input.");
            }
        }
        this.lastDashboardMessage = lastDashboardMessage;
    }

    @Override
    synchronized public void newControllerMessageAvailable(RoomControllerMessage message) {
        this.lastControllerMessage = message;
    }

    @Override
    synchronized public void newSensorStateAvailable(SensorBoardMessage message) {
        this.sensorState = message;
    }
}
