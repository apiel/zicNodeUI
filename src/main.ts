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
    drawFilledRect,
} from './lib';

open();

const red: Color = { r: 255, g: 0, b: 0, a: 255 };
setColor(red);
drawRect({ point: { x: 10, y: 10 }, h: 100, w: 100 });
drawFilledRect({ point: { x: 30, y: 30 }, h: 100, w: 100 });
drawLine({ x: 10, y: 150 }, { x: 150, y: 150 });
drawPoint({ x: 5, y: 5 });
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
