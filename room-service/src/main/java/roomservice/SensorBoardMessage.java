package roomservice;

public class SensorBoardMessage {
    private boolean motion;
    private int lightLevel;

    public SensorBoardMessage() { }

    public SensorBoardMessage(boolean motion, int lightLevel) {
        this.motion = motion;
        this.lightLevel = lightLevel;
    }

    public boolean getMotion() {
        return motion;
    }

    public int getLightLevel() {
        return lightLevel;
    }

    @Override
    public String toString() {
        return "SensorBoardMessage{" +
                "motion=" + motion +
                ", lightLevel=" + lightLevel +
                '}';
    }
}
