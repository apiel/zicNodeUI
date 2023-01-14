import bindings from 'bindings';

const zicUI = bindings('zic_ui');

export const open: () => void = zicUI.open;
export const shouldExit: () => boolean = zicUI.shouldExit;
export const close: () => void = zicUI.close;
