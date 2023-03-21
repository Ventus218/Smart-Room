package roomservice;

public enum RoomControllerInputState {
    SERVICE, BLUETOOTH;

    @Override
    public String toString() {
        switch (this) {
            case SERVICE:
                return "SERVICE";
            case BLUETOOTH:
                return "BLUETOOTH";
            default:
                throw new IllegalStateException();
        }
    }
}
