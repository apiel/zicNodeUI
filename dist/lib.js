"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.drawText = exports.drawFilledRect = exports.drawRect = exports.drawLine = exports.drawPoint = exports.setColor = exports.clear = exports.render = exports.getEvents = exports.close = exports.open = exports.getScreen = exports.rgb = void 0;
const bindings_1 = __importDefault(require("bindings"));
const zicUI = (0, bindings_1.default)('zic_ui');
function rgb(color) {
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
exports.rgb = rgb;
exports.getScreen = zicUI.getScreen;
exports.open = zicUI.open;
exports.close = zicUI.close;
exports.getEvents = zicUI.getEvents;
exports.render = zicUI.render;
exports.clear = zicUI.clear;
exports.setColor = zicUI.setColor;
exports.drawPoint = zicUI.drawPoint;
exports.drawLine = zicUI.drawLine;
exports.drawRect = zicUI.drawRect;
exports.drawFilledRect = zicUI.drawFilledRect;
exports.drawText = zicUI.drawText;
