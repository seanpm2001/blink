/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
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
 *
 */

#include "config.h"
#include "core/html/LinkRelAttribute.h"

#include "RuntimeEnabledFeatures.h"
#include "wtf/text/WTFString.h"

namespace WebCore {

LinkRelAttribute::LinkRelAttribute()
    : m_iconType(InvalidIcon)
    , m_isStyleSheet(false)
    , m_isAlternate(false)
    , m_isDNSPrefetch(false)
    , m_isLinkPrefetch(false)
    , m_isLinkSubresource(false)
    , m_isLinkPrerender(false)
    , m_isLinkNext(false)
    , m_isImport(false)
{
}

LinkRelAttribute::LinkRelAttribute(const String& rel)
    : m_iconType(InvalidIcon)
    , m_isStyleSheet(false)
    , m_isAlternate(false)
    , m_isDNSPrefetch(false)
    , m_isLinkPrefetch(false)
    , m_isLinkSubresource(false)
    , m_isLinkPrerender(false)
    , m_isLinkNext(false)
    , m_isImport(false)
{
    if (equalIgnoringCase(rel, "stylesheet")) {
        m_isStyleSheet = true;
    } else if (equalIgnoringCase(rel, "icon") || equalIgnoringCase(rel, "shortcut icon")) {
        m_iconType = Favicon;
    } else if (equalIgnoringCase(rel, "dns-prefetch")) {
        m_isDNSPrefetch = true;
    } else if (equalIgnoringCase(rel, "alternate stylesheet") || equalIgnoringCase(rel, "stylesheet alternate")) {
        m_isStyleSheet = true;
        m_isAlternate = true;
    } else if (equalIgnoringCase(rel, "import")) {
        m_isImport = true;
    } else if (equalIgnoringCase(rel, "apple-touch-icon")) {
        if (RuntimeEnabledFeatures::touchIconLoadingEnabled()) {
            m_iconType = TouchIcon;
        }
    } else if (equalIgnoringCase(rel, "apple-touch-icon-precomposed")) {
        if (RuntimeEnabledFeatures::touchIconLoadingEnabled()) {
            m_iconType = TouchPrecomposedIcon;
        }
    } else {
        // Tokenize the rel attribute and set bits based on specific keywords that we find.
        String relCopy = rel;
        relCopy.replace('\n', ' ');
        Vector<String> list;
        relCopy.split(' ', list);
        Vector<String>::const_iterator end = list.end();
        for (Vector<String>::const_iterator it = list.begin(); it != end; ++it) {
            if (equalIgnoringCase(*it, "stylesheet")) {
                m_isStyleSheet = true;
            } else if (equalIgnoringCase(*it, "alternate")) {
                m_isAlternate = true;
            } else if (equalIgnoringCase(*it, "icon")) {
                m_iconType = Favicon;
            } else if (equalIgnoringCase(*it, "prefetch")) {
                m_isLinkPrefetch = true;
            } else if (equalIgnoringCase(*it, "subresource")) {
                m_isLinkSubresource = true;
            } else if (equalIgnoringCase(*it, "prerender")) {
                m_isLinkPrerender = true;
            } else if (equalIgnoringCase(*it, "next")) {
                m_isLinkNext = true;
            } else if (equalIgnoringCase(*it, "apple-touch-icon")) {
                if (RuntimeEnabledFeatures::touchIconLoadingEnabled()) {
                    m_iconType = TouchIcon;
                }
            } else if (equalIgnoringCase(*it, "apple-touch-icon-precomposed")) {
                if (RuntimeEnabledFeatures::touchIconLoadingEnabled()) {
                    m_iconType = TouchPrecomposedIcon;
                }
            }
        }
    }
}

}
