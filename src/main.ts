import { exit } from 'process';
import { open, close, getEvents } from './lib';

open();

setInterval(() => {
    const events = getEvents();
    if (events.exit) {
        close();
        exit();
    }
}, 100);
