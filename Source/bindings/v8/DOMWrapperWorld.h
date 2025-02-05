/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#ifndef DOMWrapperWorld_h
#define DOMWrapperWorld_h

#include "bindings/v8/V8DOMActivityLogger.h"
#include "bindings/v8/V8PerContextData.h"
#include "platform/weborigin/SecurityOrigin.h"
#include <v8.h>
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "wtf/RefPtr.h"
#include "wtf/text/WTFString.h"

namespace WebCore {

class DOMDataStore;
class ScriptController;
class ExecutionContext;

enum WorldIdConstants {
    MainWorldId = 0,
    // Embedder isolated worlds can use IDs in [1, 1<<29).
    EmbedderWorldIdLimit = (1 << 29),
    ScriptPreprocessorIsolatedWorldId,
    WorkerWorldId,
};

// This class represent a collection of DOM wrappers for a specific world.
class DOMWrapperWorld : public RefCounted<DOMWrapperWorld> {
public:
    static PassRefPtr<DOMWrapperWorld> create(int worldId, int extensionGroup);

    static const int mainWorldExtensionGroup = 0;
    static PassRefPtr<DOMWrapperWorld> ensureIsolatedWorld(int worldId, int extensionGroup);
    ~DOMWrapperWorld();

    static bool isolatedWorldsExist() { return isolatedWorldCount; }
    static void getAllWorldsInMainThread(Vector<RefPtr<DOMWrapperWorld> >& worlds);

    static DOMWrapperWorld* world(v8::Handle<v8::Context> context)
    {
        ASSERT(contextHasCorrectPrototype(context));
        return V8PerContextDataHolder::from(context)->world();
    }

    // Will return null if there is no DOMWrapperWorld for the current v8::Context
    static DOMWrapperWorld* current(v8::Isolate*);
    static DOMWrapperWorld* mainWorld();

    // Associates an isolated world (see above for description) with a security
    // origin. XMLHttpRequest instances used in that world will be considered
    // to come from that origin, not the frame's.
    static void setIsolatedWorldSecurityOrigin(int worldID, PassRefPtr<SecurityOrigin>);
    static void clearIsolatedWorldSecurityOrigin(int worldID);
    SecurityOrigin* isolatedWorldSecurityOrigin();

    // Associated an isolated world with a Content Security Policy. Resources
    // embedded into the main world's DOM from script executed in an isolated
    // world should be restricted based on the isolated world's DOM, not the
    // main world's.
    //
    // FIXME: Right now, resource injection simply bypasses the main world's
    // DOM. More work is necessary to allow the isolated world's policy to be
    // applied correctly.
    static void setIsolatedWorldContentSecurityPolicy(int worldID, const String& policy);
    static void clearIsolatedWorldContentSecurityPolicy(int worldID);
    bool isolatedWorldHasContentSecurityPolicy();

    // Associate a logger with the world identified by worldId (worlId may be 0
    // identifying the main world).
    static void setActivityLogger(int worldId, PassOwnPtr<V8DOMActivityLogger>);
    static V8DOMActivityLogger* activityLogger(int worldId);

    bool isMainWorld() const { return m_worldId == MainWorldId; }
    bool isWorkerWorld() const { return m_worldId == WorkerWorldId; }
    bool isIsolatedWorld() const { return !isMainWorld() && !isWorkerWorld(); }

    int worldId() const { return m_worldId; }
    int extensionGroup() const { return m_extensionGroup; }
    DOMDataStore& domDataStore() { return *m_domDataStore; }

private:
    static unsigned isolatedWorldCount;

    DOMWrapperWorld(int worldId, int extensionGroup);
    static bool contextHasCorrectPrototype(v8::Handle<v8::Context>);

    const int m_worldId;
    const int m_extensionGroup;
    OwnPtr<DOMDataStore> m_domDataStore;
};

} // namespace WebCore

#endif // DOMWrapperWorld_h
