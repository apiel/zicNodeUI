import { exit } from 'process';
import { open, close, shouldExit } from './lib';

open();

setInterval(() => {
    if (shouldExit()) {
        close();
        exit();
    }
}, 100);
