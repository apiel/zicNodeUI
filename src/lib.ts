import bindings from 'bindings';

const zicUI = bindings('zic_ui');
interface Events {
    exit?: boolean;
    keysDown?: number[];
    keysUp?: number[];
}
export interface Color {
    r: number;
    g: number;
    b: number;
    a?: number;
}

export interface Point {
    x: number;
    y: number;
}

export interface Rect {
    point: Point;
    w: number;
    h: number;
}

export const open: () => void = zicUI.open;
export const close: () => void = zicUI.close;
export const getEvents: () => Events = zicUI.getEvents;
export const render: () => void = zicUI.render;
export const clear: (color?: Color) => void = zicUI.clear;
export const setColor: (color: Color) => number = zicUI.setColor;
export const drawPoint: (point: Point) => void = zicUI.drawPoint;
export const drawLine: (point1: Point, point2: Point) => void = zicUI.drawLine;
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
