package roomservice;

import java.time.LocalTime;
import java.util.Optional;

public class LightHistoryEntry {
    private Optional<Boolean> lightOn;
    private LocalTime time;

    public LightHistoryEntry(Optional<Boolean> lightOn, LocalTime time) {
        this.lightOn = lightOn;
        this.time = time;
    }

    public Optional<Boolean> getLightOn() {
        return lightOn;
    }

    public LocalTime getTime() {
        return time;
    }
}
