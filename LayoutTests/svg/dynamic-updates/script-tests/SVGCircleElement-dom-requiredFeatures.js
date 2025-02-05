// [Name] SVGCircleElement-dom-requiredFeatures.js
// [Expected rendering result] a series of PASS messages

createSVGTestCase();

var circleElement = createSVGElement("circle");
circleElement.setAttribute("r", "200");

rootSVGElement.appendChild(circleElement);

function repaintTest() {
    debug("Check that SVGCircleElement is initially displayed");
    shouldHaveBBox("circleElement", "400", "400");
    debug("Check that setting requiredFeatures to something invalid makes it not render");
    circleElement.setAttribute("requiredFeatures", "http://www.w3.org/TR/SVG11/feature#BogusFeature");
    shouldHaveBBox("circleElement", "0", "0");
    debug("Check that setting requiredFeatures to something valid makes it render again");
    circleElement.setAttribute("requiredFeatures", "http://www.w3.org/TR/SVG11/feature#Shape");
    shouldHaveBBox("circleElement", "400", "400");
    debug("Check that adding something valid to requiredFeatures keeps rendering the element");
    circleElement.setAttribute("requiredFeatures", "http://www.w3.org/TR/SVG11/feature#Gradient");
    shouldHaveBBox("circleElement", "400", "400");
    debug("Check that adding something invalid to requiredFeatures makes it not render");
    circleElement.setAttribute("requiredFeatures", "http://www.w3.org/TR/SVG11/feature#BogusFeature");
    shouldHaveBBox("circleElement", "0", "0");

    completeTest();
}

var successfullyParsed = true;
