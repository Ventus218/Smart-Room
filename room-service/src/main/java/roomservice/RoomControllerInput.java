package roomservice;

public class RoomControllerInput {
    private boolean lightOn;
    private int rollerPercentage;

    public RoomControllerInput(boolean lightOn, int rollerPercentage) throws IllegalArgumentException {
        if (rollerPercentage < 0 || rollerPercentage > 100) {
            throw new IllegalArgumentException("Roller percentage must be in range of 0..100 inclusive.");
        }
        this.lightOn = lightOn;
        this.rollerPercentage = rollerPercentage;
    }

    public int getRollerPercentage() {
        return rollerPercentage;
    }

    public boolean isLightOn() {
        return lightOn;
    }
}
