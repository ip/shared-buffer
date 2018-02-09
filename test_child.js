const { createSharedBuffer, detachSharedBuffer } = require('./index.js');


const SEGMENT_ID = 1024;
const SEGMENT_ELEMENTS = 10;
const SEGMENT_SIZE = SEGMENT_ELEMENTS * Float64Array.BYTES_PER_ELEMENT;

let sharedBuffer
for (let i = 0; i < 9; ++i) {
    sharedBuffer = createSharedBuffer(SEGMENT_ID, SEGMENT_SIZE);
    const arrayBuffer = new ArrayBuffer(sharedBuffer)
    detachSharedBuffer(sharedBuffer)
}

sharedBuffer = createSharedBuffer(SEGMENT_ID, SEGMENT_SIZE);
const sharedArray = new Float64Array(sharedBuffer);
console.log(sharedArray);
