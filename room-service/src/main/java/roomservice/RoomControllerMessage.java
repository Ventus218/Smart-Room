package roomservice;

import java.util.Optional;

public class RoomControllerMessage {
    private RoomControllerInputState controllerInputState;
    private Optional<Integer> rollerPercentage;
    private Optional<Boolean> lightOn;

    private RoomControllerMessage(RoomControllerInputState controllerInputState, Optional<Integer> rollerPercentage, Optional<Boolean> lightOn) throws IllegalArgumentException {
        this.controllerInputState = controllerInputState;
        switch (controllerInputState) {
            case SERVICE:
                if (rollerPercentage.isPresent() || lightOn.isPresent()) {
                    throw new IllegalArgumentException("SERVICE RoomControllerInputState doesn't allow rollerPercentage or lightOn parameters");
                }
                this.rollerPercentage = Optional.empty();
                this.lightOn = Optional.empty();
                break;
            case BLUETOOTH:
                if (rollerPercentage.isEmpty() || lightOn.isEmpty()) {
                    throw new IllegalArgumentException("BLUETOOTH RoomControllerInputState requires rollerPercentage and lightOn parameters");
                }
                if (rollerPercentage.get() < 0 || rollerPercentage.get() > 100) {
                    throw new IllegalArgumentException("Roller percentage must be in range of 0..100 inclusive.");
                }
                this.rollerPercentage = rollerPercentage;
                this.lightOn = lightOn;
                break;
        }
    }

    public static RoomControllerMessage controllerServiceInputState() {
        return new RoomControllerMessage(RoomControllerInputState.SERVICE, Optional.empty(), Optional.empty());
    }

    public static RoomControllerMessage controllerBluetoothInputState(boolean lightOn, int rollerPercentage) {
        return new RoomControllerMessage(RoomControllerInputState.BLUETOOTH, Optional.of(rollerPercentage), Optional.of(lightOn));
    }

    public RoomControllerInputState getControllerInputState() {
        return controllerInputState;
    }

    public int getRollerPercentage() {
        if (controllerInputState == RoomControllerInputState.SERVICE) {
            throw new IllegalStateException("SERVICE RoomControllerInputState rollerPercentage is undefined");
        } else {
            return rollerPercentage.get().intValue();
        }
    }

    public boolean isLightOn() {
        if (controllerInputState == RoomControllerInputState.SERVICE) {
            throw new IllegalStateException("SERVICE RoomControllerInputState lightOn is undefined");
        } else {
            return lightOn.get().booleanValue();
        }
    }

    @Override
    public String toString() {
        return "RoomControllerMessage{" +
                "controllerInputState=" + controllerInputState +
                ", rollerPercentage=" + rollerPercentage +
                ", lightOn=" + lightOn +
                '}';
    }
}
