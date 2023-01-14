import bindings from 'bindings';

const zicUI = bindings('zic_ui');

export const open: () => void = zicUI.open;
export const close: () => void = zicUI.close;

interface Events {
    exit?: boolean;
    keysDown?: number[];
    keysUp?: number[];
};

export const getEvents: () => Events = zicUI.getEvents;

export const render: () => void = zicUI.render;
export type Color = [number, number, number, number?];
export const setColor: (r: number, g: number, b: number, a?: number) => number = zicUI.setColor;
export const drawRect: (x: number, y: number, w: number, h: number, fill?: boolean) => void = zicUI.drawRect;
export const drawPoint: (x: number, y: number) => void = zicUI.drawPoint;
export const drawLine: (x1: number, y1: number, x2: number, y2: number) => void = zicUI.drawLine;
export const clear: (() => void) | ((r: number, g: number, b: number, a?: number) => void) = zicUI.clear;
