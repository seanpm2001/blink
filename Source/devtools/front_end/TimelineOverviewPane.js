/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @constructor
 * @extends {WebInspector.View}
 * @param {!WebInspector.TimelineModel} model
 */
WebInspector.TimelineOverviewPane = function(model)
{
    WebInspector.View.call(this);
    this.element.id = "timeline-overview-pane";

    this._eventDividers = [];

    this._model = model;

    this._overviewGrid = new WebInspector.OverviewGrid("timeline");
    this.element.appendChild(this._overviewGrid.element);

    this._overviewCalculator = new WebInspector.TimelineOverviewCalculator();

    model.addEventListener(WebInspector.TimelineModel.Events.RecordsCleared, this._reset, this);
    this._overviewGrid.addEventListener(WebInspector.OverviewGrid.Events.WindowChanged, this._onWindowChanged, this);
}

WebInspector.TimelineOverviewPane.Events = {
    WindowChanged: "WindowChanged"
};

WebInspector.TimelineOverviewPane.prototype = {
    wasShown: function()
    {
        this._update();
    },

    onResize: function()
    {
        this._update();
    },

    /**
     * @param {!WebInspector.TimelineOverviewBase} overviewControl
     */
    setOverviewControl: function(overviewControl)
    {
        if (this._overviewControl === overviewControl)
            return;

        var windowTimes = null;

        if (this._overviewControl) {
            windowTimes = this._overviewControl.windowTimes(this._overviewGrid.windowLeft(), this._overviewGrid.windowRight());
            this._overviewControl.detach();
        }
        this._overviewControl = overviewControl;
        this._overviewControl.show(this._overviewGrid.element);
        this._update();
        if (windowTimes)
            this.setWindowTimes(windowTimes.startTime, windowTimes.endTime);
    },

    _update: function()
    {
        delete this._refreshTimeout;

        this._overviewCalculator.setWindow(this._model.minimumRecordTime(), this._model.maximumRecordTime());
        this._overviewCalculator.setDisplayWindow(0, this._overviewGrid.clientWidth());
        this._updateWindow();
        if (this._overviewControl)
            this._overviewControl.update();
        this._overviewGrid.updateDividers(this._overviewCalculator);
        this._updateEventDividers();
    },

    _updateEventDividers: function()
    {
        var records = this._eventDividers;
        this._overviewGrid.removeEventDividers();
        var dividers = [];
        for (var i = 0; i < records.length; ++i) {
            var record = records[i];
            var positions = this._overviewCalculator.computeBarGraphPercentages(record);
            var dividerPosition = Math.round(positions.start * 10);
            if (dividers[dividerPosition])
                continue;
            var divider = WebInspector.TimelinePresentationModel.createEventDivider(record.type);
            divider.style.left = positions.start + "%";
            dividers[dividerPosition] = divider;
        }
        this._overviewGrid.addEventDividers(dividers);
    },

    /**
     * @param {!TimelineAgent.TimelineEvent} record
     */
    addRecord: function(record)
    {
        var eventDividers = this._eventDividers;
        function addEventDividers(record)
        {
            if (WebInspector.TimelinePresentationModel.isEventDivider(record))
                eventDividers.push(record);
        }
        WebInspector.TimelinePresentationModel.forAllRecords([record], addEventDividers);
        this._scheduleRefresh();
    },

    _reset: function()
    {
        this._overviewCalculator.reset();
        this._overviewGrid.reset();
        this._overviewGrid.setResizeEnabled(false);
        this._eventDividers = [];
        this._overviewGrid.updateDividers(this._overviewCalculator);
        if (this._overviewControl)
            this._overviewControl.reset();
        this._update();
    },

    /**
     * @param {!WebInspector.Event} event
     */
    _onWindowChanged: function(event)
    {
        if (this._ignoreWindowChangedEvent)
            return;
        var windowTimes = this._overviewControl.windowTimes(this._overviewGrid.windowLeft(), this._overviewGrid.windowRight());
        this._windowStartTime = windowTimes.startTime;
        this._windowEndTime = windowTimes.endTime;
        this.dispatchEventToListeners(WebInspector.TimelineOverviewPane.Events.WindowChanged, windowTimes);
    },

    /**
     * @param {number} startTime
     * @param {number} endTime
     */
    setWindowTimes: function(startTime, endTime)
    {
        this._windowStartTime = startTime;
        this._windowEndTime = endTime;
        this._updateWindow();
    },

    _updateWindow: function()
    {
        var windowBoundaries = this._overviewControl.windowBoundaries(this._windowStartTime, this._windowEndTime);
        this._ignoreWindowChangedEvent = true;
        this._overviewGrid.setWindow(windowBoundaries.left, windowBoundaries.right);
        this._overviewGrid.setResizeEnabled(this._model.records.length);
        this._ignoreWindowChangedEvent = false;
    },

    _scheduleRefresh: function()
    {
        if (this._refreshTimeout)
            return;
        if (!this.isShowing())
            return;
        this._refreshTimeout = setTimeout(this._update.bind(this), 300);
    },

    __proto__: WebInspector.View.prototype
}

/**
 * @constructor
 * @implements {WebInspector.TimelineGrid.Calculator}
 */
WebInspector.TimelineOverviewCalculator = function()
{
}

WebInspector.TimelineOverviewCalculator.prototype = {
    /**
     * @param {number} time
     * @return {number}
     */
    computePosition: function(time)
    {
        return (time - this._minimumBoundary) / this.boundarySpan() * this._workingArea + this.paddingLeft;
    },

    /**
     * @return {!{start: number, end: number}}
     */
    computeBarGraphPercentages: function(record)
    {
        var start = (WebInspector.TimelineModel.startTimeInSeconds(record) - this._minimumBoundary) / this.boundarySpan() * 100;
        var end = (WebInspector.TimelineModel.endTimeInSeconds(record) - this._minimumBoundary) / this.boundarySpan() * 100;
        return {start: start, end: end};
    },

    /**
     * @param {number=} minimum
     * @param {number=} maximum
     */
    setWindow: function(minimum, maximum)
    {
        this._minimumBoundary = minimum >= 0 ? minimum : undefined;
        this._maximumBoundary = maximum >= 0 ? maximum : undefined;
    },

    /**
     * @param {number} paddingLeft
     * @param {number} clientWidth
     */
    setDisplayWindow: function(paddingLeft, clientWidth)
    {
        this._workingArea = clientWidth - paddingLeft;
        this.paddingLeft = paddingLeft;
    },

    reset: function()
    {
        this.setWindow();
    },

    /**
     * @param {number} value
     * @param {boolean=} hires
     * @return {string}
     */
    formatTime: function(value, hires)
    {
        return Number.secondsToString(value, hires);
    },

    /**
     * @return {number}
     */
    maximumBoundary: function()
    {
        return this._maximumBoundary;
    },

    /**
     * @return {number}
     */
    minimumBoundary: function()
    {
        return this._minimumBoundary;
    },

    /**
     * @return {number}
     */
    zeroTime: function()
    {
        return this._minimumBoundary;
    },

    /**
     * @return {number}
     */
    boundarySpan: function()
    {
        return this._maximumBoundary - this._minimumBoundary;
    }
}

/**
 * @constructor
 * @extends {WebInspector.View}
 * @param {!WebInspector.TimelineModel} model
 */
WebInspector.TimelineOverviewBase = function(model)
{
    WebInspector.View.call(this);

    this._model = model;
    this._canvas = this.element.createChild("canvas", "fill");
    this._context = this._canvas.getContext("2d");
}

WebInspector.TimelineOverviewBase.prototype = {
    update: function() { },
    reset: function() { },

    /**
     * @param {number} windowLeft
     * @param {number} windowRight
     * @return {!{startTime: number, endTime: number}}
     */
    windowTimes: function(windowLeft, windowRight)
    {
        var absoluteMin = this._model.minimumRecordTime();
        var timeSpan = this._model.maximumRecordTime() - absoluteMin;
        return {
            startTime: absoluteMin + timeSpan * windowLeft,
            endTime: absoluteMin + timeSpan * windowRight
        };
    },

    /**
     * @param {number} startTime
     * @param {number} endTime
     * @return {!{left: number, right: number}}
     */
    windowBoundaries: function(startTime, endTime)
    {
        var absoluteMin = this._model.minimumRecordTime();
        var timeSpan = this._model.maximumRecordTime() - absoluteMin;
        var haveRecords = absoluteMin >= 0;
        return {
            left: haveRecords && startTime ? Math.min((startTime - absoluteMin) / timeSpan, 1) : 0,
            right: haveRecords && endTime < Infinity ? (endTime - absoluteMin) / timeSpan : 1
        }
    },

    resetCanvas: function()
    {
        this._canvas.width = this.element.clientWidth * window.devicePixelRatio;
        this._canvas.height = this.element.clientHeight * window.devicePixelRatio;
    },

    __proto__: WebInspector.View.prototype
}
