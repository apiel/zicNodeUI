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
