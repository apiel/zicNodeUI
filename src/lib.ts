import bindings from 'bindings';

const zicUI = bindings('zic_ui');

export const open: () => void = zicUI.open;
export const close: () => void = zicUI.close;

interface Events {
    exit?: boolean;
    keysDown?: number[];
    keysUp?: number[];
}

export const getEvents: () => Events = zicUI.getEvents;

export const render: () => void = zicUI.render;
export interface Color {
    r: number;
    g: number;
    b: number;
    a?: number;
}
export const setColor: (color: Color) => number = zicUI.setColor;

export interface Point {
    x: number;
    y: number;
}
export const drawPoint: (point: Point) => void = zicUI.drawPoint;
export const drawLine: (point1: Point, point2: Point) => void = zicUI.drawLine;

export interface Rect {
    point: Point;
    w: number;
    h: number;
}
export const drawRect: (rect: Rect) => void = zicUI.drawRect;
export const drawFilledRect: (rect: Rect) => void = zicUI.drawFilledRect;

export const drawText: (
    text: string,
    x: number,
    y: number,
    r: number,
    g: number,
    b: number,
    size?: number,
    a?: number,
) => void = zicUI.drawText;
export const clear: (() => void) | ((r: number, g: number, b: number, a?: number) => void) =
    zicUI.clear;
