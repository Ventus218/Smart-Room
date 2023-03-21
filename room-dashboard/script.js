
const apiRootPath = "http://localhost:8080";

const controlButton = document.getElementById("take_control_button");
const lightCheckbox = document.getElementById("light_on_checkbox");
const lightCheckboxLabel = document.getElementById("light_on_checkbox_label");
const rollerRange = document.getElementById("roller_percentage_range");
const rollerRangeLabel = document.getElementById("roller_percentage_range_label");

const sensorTableBody = document.querySelector('#sensor_table tbody');
const controllerTableBody = document.querySelector('#controller_table tbody');
const historyTableBody = document.querySelector('#light_history_table tbody');

let inputHasChanged = false;
let isFirstFetch = true;

let sensorData = null;
// { lightLevel: 2000, motion: true }

let controllerData = null;
// { inputState: "SERVICE", rollerPercentage: 0, lightOn: true }

let lightHistoryData = [];
// [
//     { time: '00:00:00', lightOn: null },
//     { time: '12:00:00', lightOn: false },
//     { time: '13:30:10', lightOn: true },
//     { time: '14:30:10', lightOn: false },
//     { time: '15:30:10', lightOn: true },
// ];

rollerRange.addEventListener("change", () => { rollerRangeLabel.innerText = "Roller percentage: " + rollerRange.value + "%"; });

function updateUI() {
    if (sensorData == null || lightHistoryData.length <= 0) { return; }
    sensorTableBody.removeChild(sensorTableBody.firstElementChild);

    const newRow = sensorTableBody.insertRow();
    const lightLevelCell = newRow.insertCell();
    lightLevelCell.innerText = sensorData.lightLevel;
    const motionCell = newRow.insertCell();
    motionCell.innerText = sensorData.motion ? "ON" : "OFF";

    if (controllerData != null) {
        controllerTableBody.removeChild(controllerTableBody.firstElementChild);

        const newRow = controllerTableBody.insertRow();
        const inputStateCell = newRow.insertCell();
        inputStateCell.innerText = controllerData.controlState;
        const lightCell = newRow.insertCell();
        lightCell.innerText = controllerData.lightOn ? "ON" : "OFF";
        const rollerPercentageCell = newRow.insertCell();
        rollerPercentageCell.innerText = controllerData.rollerPercentage + "%";

        if (controllerData.controlState != "WEB_INTERFACE") {
            controlButton.innerText = "Take control";
            lightCheckbox.disabled = true;
            rollerRange.disabled = true;
            rollerRangeLabel.style.color = "lightgray";
            lightCheckboxLabel.style.color = "lightgray"; 
            lightCheckbox.checked = controllerData.lightOn;
            rollerRange.value = controllerData.rollerPercentage;
            rollerRangeLabel.innerText = "Roller percentage: " + rollerRange.value + "%";
        } else {
            if (isFirstFetch) {
                isFirstFetch = false;
                lightCheckbox.checked = controllerData.lightOn;
                rollerRange.value = controllerData.rollerPercentage;
                rollerRangeLabel.innerText = "Roller percentage: " + rollerRange.value + "%";
            }
            controlButton.innerText = "Leave control";
            lightCheckbox.disabled = false;
            lightCheckboxLabel.style.color = "initial"; 
            rollerRange.disabled = false;
            rollerRangeLabel.style.color = "initial";
        }
        controlButton.disabled = false;
    }

    while (historyTableBody.hasChildNodes()) {
        historyTableBody.removeChild(historyTableBody.firstChild);
    }

    lightHistoryData.forEach((item, index) => {
        const row = historyTableBody.insertRow();
        const timeCell = row.insertCell();
        const stateCell = row.insertCell();
        timeCell.innerText = item.time;
        stateCell.innerText = item.lightOn == null ? "???" : (item.lightOn ? "ON" : "OFF");
    });
}

function pollServerAndUpdateUI() {
    axios.get(apiRootPath + "/dashboard-data").then( res => {
        sensorData = res.data.sensorData;
        controllerData = res.data.controllerData;
        lightHistoryData = res.data.lightHistoryData;
        updateUI();
    });    
}

pollServerAndUpdateUI();

let timer = window.setInterval(pollServerAndUpdateUI, 5000);

function inputChanged() {
    inputHasChanged = true;
    controlButton.disabled = true;
}

rollerRange.addEventListener("change", inputChanged);
lightCheckbox.addEventListener("change", inputChanged);
controlButton.addEventListener("click", () => sendInput(controllerData.controlState != "WEB_INTERFACE") );

function sendInput(takeControl) {
    controlButton.disabled = true;

    message = {
        dashboardControl: takeControl,
        setLightOn: lightCheckbox.checked,
        setRollerPercentage: rollerRange.value
    }

    axios.post(apiRootPath + "/controller", message).then( res => {
        
    });
}

let inputTimer = window.setInterval(() =>{
    if (inputHasChanged) {
        sendInput(true);
        inputHasChanged = false;
    }
}, 1000);
