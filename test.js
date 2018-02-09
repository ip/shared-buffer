const { spawn } = require('child_process')
const path = require('path')

const { createSharedBuffer } = require('./index.js');


const SEGMENT_ID = 1024;
const SEGMENT_ELEMENTS = 10;
const SEGMENT_SIZE = SEGMENT_ELEMENTS * Float64Array.BYTES_PER_ELEMENT;


const sharedBuffer = createSharedBuffer(SEGMENT_ID, SEGMENT_SIZE, true);
const sharedArray = new Float64Array(sharedBuffer);
for(let i = 0; i < SEGMENT_ELEMENTS; i++)
  sharedArray[i] = 1/i;


const child = spawn('node', [
  // '--inspect-brk=9229',
  'test_child.js',
], {
  stdio: 'inherit',
})

child.on('error', err => {
  console.error('Error on child process:', err)
})

child.on('exit', (code, signal) => {
  console.error(`Child process exited with code ${code} and signal ${signal}`)
})
