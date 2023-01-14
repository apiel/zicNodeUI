import { exit } from 'process';
import { open, close, getEvents } from './lib';

open();

setInterval(() => {
    const events = getEvents();
    if (events.exit) {
        close();
        exit();
    } else if (events.keysDown || events.keysUp) {
        console.log('events', events);
    }
}, 100);
