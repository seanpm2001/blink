description('Tests using null values for some of the event properties.');

var mockEvent;
function setMockOrientation(alpha, beta, gamma, absolute) {
    mockEvent = {alpha: alpha, beta: beta, gamma: gamma, absolute: absolute};
    if (window.testRunner)
        testRunner.setMockDeviceOrientation(
            null != mockEvent.alpha, null == mockEvent.alpha ? 0 : mockEvent.alpha,
            null != mockEvent.beta, null == mockEvent.beta ? 0 : mockEvent.beta,
            null != mockEvent.gamma, null == mockEvent.gamma ? 0 : mockEvent.gamma,
            null != mockEvent.absolute, null == mockEvent.absolute ? false : mockEvent.absolute);
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
    setTimeout(function(){initSecondListener();}, 0);
}

function initSecondListener() {
    setMockOrientation(1.1, null, null, true);
    window.addEventListener('deviceorientation', secondListener);
}

function secondListener(event) {
    checkOrientation(event);
    window.removeEventListener('deviceorientation', secondListener);
    setTimeout(function(){initThirdListener();}, 0);
}

function initThirdListener() {
    setMockOrientation(null, 2.2, null, true);
    window.addEventListener('deviceorientation', thirdListener);
}

function thirdListener(event) {
    checkOrientation(event);
    window.removeEventListener('deviceorientation', thirdListener);
    setTimeout(function(){initFourthListener();}, 0);
}

function initFourthListener() {
    setMockOrientation(null, null, 3.3, true);
    window.addEventListener('deviceorientation', fourthListener);
}

function fourthListener(event) {
    checkOrientation(event);
    finishJSTest();
}

setMockOrientation(null, null, null, false);
window.addEventListener('deviceorientation', firstListener);

window.jsTestIsAsync = true;
