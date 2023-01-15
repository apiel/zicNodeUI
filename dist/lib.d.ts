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
export declare function rgb(color: string): Color;
export interface Screen {
    size: Size;
    position: Point;
}
export declare const getScreen: () => Screen;
export declare const open: () => void;
export declare const close: () => void;
export declare const getEvents: () => Events;
export declare const render: () => void;
export declare const clear: (color?: Color) => void;
export declare const setColor: (color: Color) => number;
export declare const drawPoint: (position: Point) => void;
export declare const drawLine: (position1: Point, position2: Point) => void;
export declare const drawRect: (rect: Rect) => void;
export declare const drawFilledRect: (rect: Rect) => void;
export declare const drawText: (text: string, position: Point, options?: TextOptions) => void;
export {};
//# sourceMappingURL=lib.d.ts.map