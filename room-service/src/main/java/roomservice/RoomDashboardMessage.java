package roomservice;

public class RoomDashboardMessage {
    private boolean dashboardControl;
    private boolean setLightOn;
    private int setRollerPercentage;

    public RoomDashboardMessage() { }

    private RoomDashboardMessage(boolean dashboardControl, boolean setLightOn, int setRollerPercentage) {
        // check arguments
        this.dashboardControl = dashboardControl;
        this.setLightOn = setLightOn;
        this.setRollerPercentage = setRollerPercentage;
    }

    public static RoomDashboardMessage dashboardControlMessage(boolean setLightOn, int setRollerPercentage) {
        return new RoomDashboardMessage(true, setLightOn, setRollerPercentage);
    }

    public static RoomDashboardMessage dashboardLeftControlMessage() {
        return new RoomDashboardMessage(false, false, 0);
    }

    public boolean isDashboardControl() {
        return dashboardControl;
    }

    public boolean isSetLightOn() {
        return setLightOn;
    }

    public int getSetRollerPercentage() {
        return setRollerPercentage;
    }

    @Override
    public String toString() {
        return "RoomDashboardMessage{" +
                "dashboardControl=" + dashboardControl +
                ", setLightOn=" + setLightOn +
                ", setRollerPercentage=" + setRollerPercentage +
                '}';
    }
}
