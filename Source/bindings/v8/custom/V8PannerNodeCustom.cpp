/*
 * Copyright (C) 2012, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#if ENABLE(WEB_AUDIO)
#include "V8PannerNode.h"

#include "bindings/v8/ExceptionState.h"
#include "bindings/v8/V8Binding.h"
#include "modules/webaudio/PannerNode.h"

namespace WebCore {

void V8PannerNode::panningModelAttributeSetterCustom(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "panningModel", "PannerNode", info.Holder(), info.GetIsolate());
    PannerNode* imp = V8PannerNode::toNative(info.Holder());

    if (value->IsNumber()) {
        uint32_t model = toUInt32(value, exceptionState);
        if (exceptionState.throwIfNeeded())
            return;
        if (!imp->setPanningModel(model)) {
            exceptionState.throwTypeError("Illegal panningModel");
            exceptionState.throwIfNeeded();
        }
        return;
    }

    if (value->IsString()) {
        String model = toCoreString(value.As<v8::String>());
        if (model == "equalpower" || model == "HRTF" || model == "soundfield") {
            imp->setPanningModel(model);
            return;
        }
    }

    exceptionState.throwTypeError("Illegal panningModel");
    exceptionState.throwIfNeeded();
}

void V8PannerNode::distanceModelAttributeSetterCustom(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "distanceModel", "PannerNode", info.Holder(), info.GetIsolate());
    PannerNode* imp = V8PannerNode::toNative(info.Holder());

    if (value->IsNumber()) {
        uint32_t model = toUInt32(value, exceptionState);
        if (exceptionState.throwIfNeeded())
            return;
        if (!imp->setDistanceModel(model)) {
            exceptionState.throwTypeError("Illegal distanceModel");
            exceptionState.throwIfNeeded();
        }
        return;
    }

    if (value->IsString()) {
        String model = toCoreString(value.As<v8::String>());
        if (model == "linear" || model == "inverse" || model == "exponential") {
            imp->setDistanceModel(model);
            return;
        }
    }

    exceptionState.throwTypeError("Illegal distanceModel");
    exceptionState.throwIfNeeded();
}

} // namespace WebCore

#endif // ENABLE(WEB_AUDIO)
