/*
 * Copyright (C) 2014 Google Inc. All rights reserved.
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

#include "config.h"
#include "heap/Visitor.h"

#include "heap/Handle.h"
#include "heap/Heap.h"

namespace WebCore {

#ifndef NDEBUG
void Visitor::checkTypeMarker(const void* payload, const char* marker)
{
    FinalizedHeapObjectHeader::fromPayload(payload)->checkHeader();
    ASSERT(FinalizedHeapObjectHeader::fromPayload(payload)->typeMarker() == marker);
}

#define DEFINE_VISITOR_CHECK_MARKER(Type)                                    \
    void Visitor::checkTypeMarker(const Type* payload, const char* marker)   \
    {                                                                        \
        HeapObjectHeader::fromPayload(payload)->checkHeader();               \
        Type* object = const_cast<Type*>(payload);                           \
        Address addr = pageHeaderAddress(reinterpret_cast<Address>(object)); \
        BaseHeapPage* page = reinterpret_cast<BaseHeapPage*>(addr);          \
        ASSERT(page->gcInfo());                                              \
        ASSERT(page->gcInfo()->m_typeMarker == marker);                      \
    }

FOR_EACH_TYPED_HEAP(DEFINE_VISITOR_CHECK_MARKER)
#undef DEFINE_VISITOR_CHECK_MARKER
#endif

#define DEFINE_DO_NOTHING_TRAIT(type)                  \
const GCInfo GCInfoTrait<type>::info = {               \
    #type,                                             \
    doNothingTrace,                                    \
    0, /* no finalizer method */                       \
    false                                              \
};

ITERATE_DO_NOTHING_TYPES(DEFINE_DO_NOTHING_TRAIT)

}
