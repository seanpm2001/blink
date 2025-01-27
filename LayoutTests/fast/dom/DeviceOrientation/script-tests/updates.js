description('Tests that updates to the orientation causes new events to fire.');

var mockEvent;
function setMockOrientation(alpha, beta, gamma, absolute) {
    mockEvent = {alpha: alpha, beta: beta, gamma: gamma, absolute: absolute};
    if (window.testRunner)
        testRunner.setMockDeviceOrientation(true, mockEvent.alpha, true, mockEvent.beta, true, mockEvent.gamma, true, mockEvent.absolute);
    else
        debug('This test can not be run without the TestRunner');
}

var deviceOrientationEvent;
function checkOrientation(event) {
    deviceOrientationEvent = event;
    shouldBe('deviceOrientationEvent.alpha', 'mockEvent.alpha');
    shouldBe('deviceOrientationEvent.beta', 'mockEvent.beta');
    shouldBe('deviceOrientationEvent.gamma', 'mockEvent.gamma');
    shouldBe('deviceOrientationEvent.absolute', 'mockEvent.absolute');
}

function firstListener(event) {
    checkOrientation(event);
    window.removeEventListener('deviceorientation', firstListener);
    setTimeout(function(){initUpdateListener();}, 0);
}

function initUpdateListener() {
    setMockOrientation(11.1, 22.2, 33.3, true);
    window.addEventListener('deviceorientation', updateListener);
}

function updateListener(event) {
    checkOrientation(event);
    finishJSTest();
}

setMockOrientation(1.1, 2.2, 3.3, true);
window.addEventListener('deviceorientation', firstListener);

window.jsTestIsAsync = true;
