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
export declare const open: () => void;
export declare const close: () => void;
export declare const getEvents: () => Events;
export declare const render: () => void;
export declare const clear: (color?: Color) => void;
export declare const setColor: (color: Color) => number;
export declare const drawPoint: (point: Point) => void;
export declare const drawLine: (point1: Point, point2: Point) => void;
export declare const drawRect: (rect: Rect) => void;
export declare const drawFilledRect: (rect: Rect) => void;
export declare const drawText: (text: string, x: number, y: number, r: number, g: number, b: number, size?: number, a?: number) => void;
export {};
//# sourceMappingURL=lib.d.ts.map