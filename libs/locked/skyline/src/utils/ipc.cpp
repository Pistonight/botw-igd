#include "skyline/utils/ipc.hpp"

#include "skyline/utils/cpputils.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include "skyline/nx/sf/cmif.h"

#ifdef __cplusplus
}
#endif

namespace skyline::utils {

nn::Result nnServiceCreate(Service* srv, const char* name) {
    // open session
    nn::sf::hipc::InitializeHipcServiceResolution();
    nn::svc::Handle svcHandle;
    nn::sf::hipc::ConnectToHipcService(&svcHandle, name);
    nn::sf::hipc::FinalizeHipcServiceResolution();

    void* base = nn::sf::hipc::GetMessageBufferOnTls();

    cmifMakeControlRequest(base, 3, 0);
    NN_ABORT_IF_FAIL(nn::sf::hipc::SendSyncRequest(svcHandle, base, 0x100));

    CmifResponse resp = {};
    //TODO fix manually expanded macro
    const auto r2 = cmifParseResponse(&resp, base, false, sizeof(u16));
    if(R_FAILED(r2)){
        return nn::result::detail::ResultInternalAccessor::ConstructResult(r2);
    }

    // build srv obj
    srv->session = svcHandle.handle;
    srv->own_handle = 1;
    srv->object_id = 0;
    srv->pointer_buffer_size = *(u16*)resp.data;

    return nn::ResultSuccess();
}

void nnServiceClose(Service* s) {
    void* base = nn::sf::hipc::GetMessageBufferOnTls();

    if (s->own_handle || s->object_id) {
        cmifMakeCloseRequest(base, s->own_handle ? 0 : s->object_id);
        nn::sf::hipc::SendSyncRequest(s->session, base, 0x100);
        if (s->own_handle) nn::sf::hipc::CloseClientSessionHandle(s->session);
    }
    *s = (Service){};
}

nn::Result Ipc::getOwnProcessHandle(Handle* handleOut) {
    Service srv;
    u64 pid;

    svcGetProcessId(&pid, 0xFFFF8001);

    nnServiceCreate(&srv, "pm:dmnt");

    Handle tmp_handle;

    struct {
        u64 loc;
        u8 status;
    } out;

    nn::Result result = nnServiceDispatchInOut(&srv, 65000, pid, out, .out_handle_attrs = {SfOutHandleAttr_HipcCopy},
                                       .out_handles = &tmp_handle, );

    nnServiceClose(&srv);

    if (result.IsSuccess()) {
        if (handleOut) {
            *handleOut = tmp_handle;
        } else {
            svcCloseHandle(tmp_handle);
        }
    }

    return nn::ResultSuccess();
}
}  // namespace skyline::utils
