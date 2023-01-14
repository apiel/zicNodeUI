import { exit } from 'process';
import { open, close, getEvents, drawRect, render, setColor, Color } from './lib';

open();

const red: Color = [255, 0, 0];
setColor(...red);
drawRect(10, 10, 100, 100, true);
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
