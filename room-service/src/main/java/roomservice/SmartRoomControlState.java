package roomservice;

public enum SmartRoomControlState {
    AUTOMATIC, BLUETOOTH, WEB_INTERFACE;

    @Override
    public String toString() {
        switch (this) {
            case AUTOMATIC:
                return "AUTOMATIC";
            case BLUETOOTH:
                return "BLUETOOTH";
            case WEB_INTERFACE:
                return "WEB_INTERFACE";
            default:
                throw new IllegalStateException();
        }
    }
}
