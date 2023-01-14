import { exit } from 'process';
import { open, close, getEvents, rgb, drawRect, render } from './lib';

open();

const red = rgb(255, 0, 0);
drawRect(10, 10, 100, 100, red, true);
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
