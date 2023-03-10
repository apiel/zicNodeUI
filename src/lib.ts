import bindings from 'bindings';

const zicUI = bindings('zic_ui');
export interface Events {
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

export interface Size {
    w: number;
    h: number;
}

export interface Rect {
    position: Point;
    size: Size;
}

export interface TextOptions {
    color?: Color;
    size?: number;
    font?: string;
}

export function rgb(color: string): Color {
    const rgb = color
        .replace(/#/, '')
        .match(/.{1,2}/g)
        ?.map((x) => parseInt(x, 16));
    if (!rgb) {
        throw new Error(`Invalid color: ${color}. Must be in the format #RRGGBB`);
    }
    const [r, g, b] = rgb;
    return { r, g, b };
}

export interface Screen {
    size: Size;
    position: Point;
}

export interface ScreenOptions {
    size?: Size;
    position?: Point;
}

export interface TextReturn {
    size: Size;
    position: Point;
}

export const getScreen: () => Screen = zicUI.getScreen;
export const open: (options?: ScreenOptions) => void = zicUI.open;
export const close: () => void = zicUI.close;
export const minimize: () => void = zicUI.minimize;
export const getEvents: () => Events = zicUI.getEvents;
export const render: () => void = zicUI.render;
export const clear: (color?: Color) => void = zicUI.clear;
export const setColor: (color: Color) => number = zicUI.setColor;
export const drawPoint: (position: Point) => void = zicUI.drawPoint;
export const drawLine: (position1: Point, position2: Point) => void = zicUI.drawLine;
export const drawRect: (rect: Rect) => void = zicUI.drawRect;
export const drawFilledRect: (rect: Rect) => void = zicUI.drawFilledRect;
export const drawText: (text: string, position: Point, options?: TextOptions) => TextReturn =
    zicUI.drawText;
