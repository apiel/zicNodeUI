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
export const rgb: (r: number, g: number, b: number) => number = zicUI.rgb;
export const drawRect: (x: number, y: number, w: number, h: number, color: number, fill?: boolean) => void = zicUI.drawRect;
