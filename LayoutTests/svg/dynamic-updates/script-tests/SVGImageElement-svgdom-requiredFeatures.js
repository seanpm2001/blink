// [Name] SVGImageElement-svgdom-requiredFeatures.js
// [Expected rendering result] a series of PASS messages

createSVGTestCase();

var imageElement = createSVGElement("image");
imageElement.setAttribute("width", "200");
imageElement.setAttribute("height", "200");

rootSVGElement.appendChild(imageElement);

function repaintTest() {
    debug("Check that SVGImageElement is initially displayed");
    shouldHaveBBox("imageElement", "200", "200");
    debug("Check that setting requiredFeatures to something invalid makes it not render");
    imageElement.requiredFeatures.appendItem("http://www.w3.org/TR/SVG11/feature#BogusFeature");
    shouldHaveBBox("imageElement", "0", "0");
    debug("Check that setting requiredFeatures to something valid makes it render again");
    imageElement.requiredFeatures.replaceItem("http://www.w3.org/TR/SVG11/feature#Shape", 0);
    shouldHaveBBox("imageElement", "200", "200");
    debug("Check that adding something valid to requiredFeatures keeps rendering the element");
    imageElement.requiredFeatures.appendItem("http://www.w3.org/TR/SVG11/feature#Gradient");
    shouldHaveBBox("imageElement", "200", "200");
    debug("Check that adding something invalid to requiredFeatures makes it not render");
    imageElement.requiredFeatures.appendItem("http://www.w3.org/TR/SVG11/feature#BogusFeature");
    shouldHaveBBox("imageElement", "0", "0");

    completeTest();
}

var successfullyParsed = true;
