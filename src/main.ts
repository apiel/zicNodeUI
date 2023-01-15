import { exit } from 'process';
import {
    open,
    close,
    getEvents,
    drawRect,
    render,
    setColor,
    Color,
    drawLine,
    drawPoint,
    drawText,
} from './lib';

open();

const red: Color = { r: 255, g: 0, b: 0, a: 255 };
setColor(red);
drawRect(10, 10, 100, 100, true);
drawLine(10, 150, 150, 150);
drawPoint({x: 5, y: 5});
drawText('Hello World', 10, 10, 255, 255, 255, 20);
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
