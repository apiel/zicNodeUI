export declare const open: () => void;
export declare const close: () => void;
interface Events {
    exit?: boolean;
    keysDown?: number[];
    keysUp?: number[];
}
export declare const getEvents: () => Events;
export declare const render: () => void;
export declare const rgb: (r: number, g: number, b: number) => number;
export declare const drawRect: (x: number, y: number, w: number, h: number, color: number, fill?: boolean) => void;
export {};
//# sourceMappingURL=lib.d.ts.map