/*
 * Copyright (C) 2018-2019 Jolla Ltd.
 * Copyright (C) 2018-2019 Slava Monich <slava.monich@jolla.com>
 *
 * You may use this file under the terms of BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the names of the copyright holders nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GBINDER_IPC_H
#define GBINDER_IPC_H

#include "gbinder_types_p.h"

#include <glib-object.h>

typedef struct gbinder_ipc_priv GBinderIpcPriv;
struct gbinder_ipc {
    GObject object;
    GBinderIpcPriv* priv;
    GBinderDriver* driver;
    GUtilIdlePool* pool;
    const char* dev;
};

typedef struct gbinder_ipc_tx GBinderIpcTx;

typedef
void
(*GBinderIpcTxFunc)(
    const GBinderIpcTx* tx);

struct gbinder_ipc_tx {
    gulong id;
    gboolean cancelled;
    GBinderIpc* ipc;
    void* user_data;
};

typedef
void
(*GBinderIpcReplyFunc)(
    GBinderIpc* ipc,
    GBinderRemoteReply* reply,
    int status,
    void* user_data);

G_GNUC_INTERNAL
GBinderIpc*
gbinder_ipc_new(
   const char* dev,
   const GBinderRpcProtocol* protocol);

G_GNUC_INTERNAL
GBinderIpc*
gbinder_ipc_ref(
    GBinderIpc* ipc);

G_GNUC_INTERNAL
void
gbinder_ipc_unref(
    GBinderIpc* ipc);

G_GNUC_INTERNAL
void
gbinder_ipc_looper_check(
   GBinderIpc* ipc);

G_GNUC_INTERNAL
GBinderObjectRegistry*
gbinder_ipc_object_registry(
    GBinderIpc* ipc);

G_GNUC_INTERNAL
void
gbinder_ipc_register_local_object(
    GBinderIpc* ipc,
    GBinderLocalObject* obj);

G_GNUC_INTERNAL
GBinderRemoteObject*
gbinder_ipc_get_remote_object(
    GBinderIpc* ipc,
    guint32 handle,
    gboolean maybe_dead);

G_GNUC_INTERNAL
void
gbinder_ipc_invalidate_remote_handle(
    GBinderIpc* ipc,
    guint32 handle);

G_GNUC_INTERNAL
GBinderRemoteReply*
gbinder_ipc_transact_sync_reply(
    GBinderIpc* ipc,
    guint32 handle,
    guint32 code,
    GBinderLocalRequest* req,
    int* status);

G_GNUC_INTERNAL
int
gbinder_ipc_transact_sync_oneway(
    GBinderIpc* ipc,
    guint32 handle,
    guint32 code,
    GBinderLocalRequest* req);

G_GNUC_INTERNAL
gulong
gbinder_ipc_transact(
    GBinderIpc* ipc,
    guint32 handle,
    guint32 code,
    guint32 flags, /* GBINDER_TX_FLAG_xxx */
    GBinderLocalRequest* req,
    GBinderIpcReplyFunc func,
    GDestroyNotify destroy,
    void* user_data);

G_GNUC_INTERNAL
gulong
gbinder_ipc_transact_custom(
    GBinderIpc* ipc,
    GBinderIpcTxFunc exec,
    GBinderIpcTxFunc done,
    GDestroyNotify destroy,
    void* user_data);

G_GNUC_INTERNAL
void
gbinder_ipc_cancel(
    GBinderIpc* ipc,
    gulong id);

/* Internal for GBinderLocalObject */
G_GNUC_INTERNAL
void
gbinder_ipc_local_object_disposed(
    GBinderIpc* self,
    GBinderLocalObject* obj);

/* Internal for GBinderRemoteObject */
G_GNUC_INTERNAL
void
gbinder_ipc_remote_object_disposed(
    GBinderIpc* self,
    GBinderRemoteObject* obj);

/* Declared for unit tests */
G_GNUC_INTERNAL
__attribute__((destructor))
void
gbinder_ipc_exit(
    void);

#endif /* GBINDER_IPC_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
