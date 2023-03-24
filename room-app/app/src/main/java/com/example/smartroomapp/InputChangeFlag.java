package com.example.smartroomapp;

public enum InputChangeFlag {
    NO_CHANGE,
    BT_CONN,
    BT_DISC;

    public int getValue() {
        switch (this) {
            case NO_CHANGE: return 0;
            case BT_CONN: return 1;
            case BT_DISC: return 2;
        }
        return -1;
    }
}
