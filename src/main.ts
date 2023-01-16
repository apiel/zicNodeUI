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
    rgb,
    clear,
    getScreen,
} from './lib';

open({ position: {x: 10, y: 10}});
console.log('screen', getScreen());

const red: Color = { r: 255, g: 0, b: 0 };
const white: Color = { r: 255, g: 255, b: 255 };
const blue: Color = rgb('#6189cb');
const background: Color = rgb('#21252b');
clear(background);
setColor(red);
drawRect({ position: { x: 10, y: 10 }, size: { h: 100, w: 100 } });
drawFilledRect({ position: { x: 30, y: 30 }, size: { h: 100, w: 100 } });
setColor(blue);
drawLine({ x: 10, y: 150 }, { x: 150, y: 150 });
drawPoint({ x: 5, y: 5 });
console.log('drawText', drawText('Hello World', { x: 10, y: 10 }, { color: white, size: 20 }));
drawText(
    'Hello',
    { x: 120, y: 10 },
    { color: blue, font: '/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf' },
);
drawText('without options', { x: 120, y: 30 });
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
