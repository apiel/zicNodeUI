export declare const open: () => void;
export declare const close: () => void;
interface Events {
    exit?: boolean;
    keysDown?: number[];
    keysUp?: number[];
}
export declare const getEvents: () => Events;
export declare const render: () => void;
export interface Color {
    r: number;
    g: number;
    b: number;
    a?: number;
}
export declare const setColor: (color: Color) => number;
export interface Point {
    x: number;
    y: number;
}
export declare const drawPoint: (point: Point) => void;
export declare const drawLine: (x1: number, y1: number, x2: number, y2: number) => void;
export declare const drawRect: (x: number, y: number, w: number, h: number, fill?: boolean) => void;
export declare const drawText: (text: string, x: number, y: number, r: number, g: number, b: number, size?: number, a?: number) => void;
export declare const clear: (() => void) | ((r: number, g: number, b: number, a?: number) => void);
export {};
//# sourceMappingURL=lib.d.ts.map