"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const process_1 = require("process");
const lib_1 = require("./lib");
(0, lib_1.open)({ position: { x: 10, y: 10 } });
console.log('screen', (0, lib_1.getScreen)());
const red = { r: 255, g: 0, b: 0 };
const white = { r: 255, g: 255, b: 255 };
const blue = (0, lib_1.rgb)('#6189cb');
const background = (0, lib_1.rgb)('#21252b');
(0, lib_1.clear)(background);
(0, lib_1.setColor)(red);
(0, lib_1.drawRect)({ position: { x: 10, y: 10 }, size: { h: 100, w: 100 } });
(0, lib_1.drawFilledRect)({ position: { x: 30, y: 30 }, size: { h: 100, w: 100 } });
(0, lib_1.setColor)(blue);
(0, lib_1.drawLine)({ x: 10, y: 150 }, { x: 150, y: 150 });
(0, lib_1.drawPoint)({ x: 5, y: 5 });
console.log('drawText', (0, lib_1.drawText)('Hello World', { x: 10, y: 10 }, { color: white, size: 20 }));
(0, lib_1.drawText)('Hello', { x: 120, y: 10 }, { color: blue, font: '/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf' });
(0, lib_1.drawText)('without options', { x: 120, y: 30 });
(0, lib_1.render)();
setInterval(() => {
    const events = (0, lib_1.getEvents)();
    if (events.exit || events.keysDown?.includes(41)) {
        (0, lib_1.close)();
        (0, process_1.exit)();
    }
    else if (events.keysDown || events.keysUp) {
        console.log('events', events);
    }
}, 10);
