package com.example.smartroomapp;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class ControllerInputActivity extends AppCompatActivity {
    private boolean hasControl = false;

    private Button controlButton;
    private Switch lightSwitch;
    private SeekBar rollerSlider;

    private OutputStream emulatedBluetoothOutputStream;
    private EmulatedClientConnectionThread connectionThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controller_input);

        controlButton = findViewById(R.id.control_button);

        lightSwitch = findViewById(R.id.light_switch);
        lightSwitch.setEnabled(false);

        rollerSlider = findViewById(R.id.roller_slider);
        rollerSlider.setEnabled(false);

        controlButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                hasControl = !hasControl;
                sendState(hasControl ? InputChangeFlag.BT_CONN : InputChangeFlag.BT_DISC);
                if (hasControl) {
                    controlButton.setText("Leave Control");
                    lightSwitch.setEnabled(true);
                    rollerSlider.setEnabled(true);
                } else {
                    controlButton.setText("Take Control");
                    lightSwitch.setEnabled(false);
                    rollerSlider.setEnabled(false);
                }
            }
        });

        lightSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                sendState();
            }
        });

        rollerSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                sendState();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) { }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) { }
        });
    }

    private void sendState() {
        sendState(InputChangeFlag.NO_CHANGE);
    }

    private void sendState(InputChangeFlag changeFlag) {
        new Thread(() -> {
            try {
                // Message encoding: "inputChangeFlag;inputSource;lightOn;rollerPercentage" as "int;int;int;int"
                String message = changeFlag.getValue() + ";1;" + (lightSwitch.isChecked() ? "1" : "0") + ";" + rollerSlider.getProgress() + "\n";
                emulatedBluetoothOutputStream.write(message.getBytes(StandardCharsets.UTF_8));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }

    @Override
    protected void onStart() {
        super.onStart();
        connectionThread = new EmulatedClientConnectionThread(this::manageConnectedSocket);
        connectionThread.start();
    }

    private void manageConnectedSocket(Socket socket) {
        try {
            emulatedBluetoothOutputStream = socket.getOutputStream();
            Log.i(C.TAG, "Connection successful!");
        } catch (IOException e) {
            Log.e(C.TAG, "Error occurred when creating output stream", e);
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        hasControl = false;
        controlButton.setText("Take Control");
        lightSwitch.setEnabled(false);
        rollerSlider.setEnabled(false);
        new Thread(() -> {
            try {
                // Message encoding: "inputChangeFlag;inputSource;lightOn;rollerPercentage" as "int;int;int;int"
                String message = InputChangeFlag.BT_DISC.getValue() + ";1;" + (lightSwitch.isChecked() ? "1" : "0") + ";" + rollerSlider.getProgress() + "\n";
                emulatedBluetoothOutputStream.write(message.getBytes(StandardCharsets.UTF_8));
                connectionThread.cancel();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }
}