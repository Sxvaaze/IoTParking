let slots = {
    '1': undefined,
    '2': undefined,
    '3': undefined
}

let EntryTimes = [];
let ExitTimes = [];

const user = "your-username";
const key = "your-key";

const mqtt = require('mqtt');
const url = `mqtts://${user}:${key}@io.adafruit.com`;
const client = mqtt.connect(url);

client.on('connect', function () {
    console.log('Connected');
    let feeds = ["Slot1", "Slot2", "Slot3", "EntryTime", "ExitTime"];
    feeds.forEach((f) => {
        client.subscribe(`${user}/feeds/${f}`, () => {
            console.log(`Subscribed to topic ${f}`);
        })
    })
});

client.on('message', (topic, payload) => {
    const topic_num = topic.split("/")[topic.split("/").length - 1];
    const date = new Date();
    const month = ('0' + (date.getMonth() + 1)).slice(-2);
    const day = ('0' + date.getDate()).slice(-2);
    const year = date.getUTCFullYear();

    if (topic_num === "EntryTime") {
        EntryTimes.push(year + "/" + month + "/" + day + " " + payload.toString());
    }
    else if (topic_num === "ExitTime") {
        ExitTimes.push(year + "/" + month + "/" + day + " " + payload.toString());
    }
    else {
        slots[topic_num[topic_num.length - 1]] = payload.toString();
    }
});

client.on('error', function (error) {
    console.log('An error occurred. Error: ', error);
});

module.exports = {slots, EntryTimes, ExitTimes};
