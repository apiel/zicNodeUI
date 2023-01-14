import { exit } from 'process';
import { open, close, getEvents, drawRect, render, setColor, Color, drawLine, drawPoint } from './lib';

open();

const red: Color = [255, 0, 0];
setColor(...red);
drawRect(10, 10, 100, 100, true);
drawLine(10, 150, 150, 150);
drawPoint(5, 5);
render();

setInterval(() => {
    const events = getEvents();
    if (events.exit || events.keysDown?.includes(41)) {
        close();
        exit();
    } else if (events.keysDown || events.keysUp) {
        console.log('events', events);
    }
}, 10);
