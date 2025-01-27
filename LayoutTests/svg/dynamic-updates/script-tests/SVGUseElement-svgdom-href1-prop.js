// [Name] SVGUseElement-svgdom-href1-prop.js
// [Expected rendering result] A use element first with an internal then with an external referenced document - and a series of PASS messages

description("Tests dynamic updates of the 'href' property of the SVGUseElement object")
createSVGTestCase();

var defsElement = createSVGElement("defs");
rootSVGElement.appendChild(defsElement);

var useElement = createSVGElement("use");
useElement.setAttribute("x", "10");
useElement.setAttribute("y", "10");
useElement.setAttribute("onload", "completeTest()");
useElement.setAttributeNS(xlinkNS, "xlink:href", "#MyRect");

var rectElement = createSVGElement("rect");
rectElement.setAttribute("id", "MyRect");
rectElement.setAttribute("x", "64");
rectElement.setAttribute("y", "0");
rectElement.setAttribute("width", "64");
rectElement.setAttribute("height", "64");
rectElement.setAttribute("fill", "red");

defsElement.appendChild(rectElement);
defsElement.appendChild(useElement);

rootSVGElement.setAttribute("height", "200");
rootSVGElement.appendChild(useElement);

shouldBeEqualToString("useElement.href.baseVal", "#MyRect");

function repaintTest() {
    useElement.href.baseVal = "../custom/resources/rgb.svg#G";
    shouldBeEqualToString("useElement.href.baseVal", "../custom/resources/rgb.svg#G");
}

var successfullyParsed = true;
