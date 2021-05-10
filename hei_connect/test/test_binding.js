const HeiConnect = require("../lib/binding.js");
const assert = require("assert");

assert(HeiConnect, "The expected function is undefined");

function testBasic()
{
    const result =  HeiConnect("hello");
    assert.strictEqual(result, "world", "Unexpected value returned");
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");