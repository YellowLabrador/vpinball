/*** Autogenerated by WIDL 9.18 from FlexUDMD.idl - Do not edit ***/

#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#elif defined(__cplusplus)

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    EXTERN_C const type DECLSPEC_SELECTANY name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#else

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    const type DECLSPEC_SELECTANY name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif

#ifdef __cplusplus
extern "C" {
#endif

MIDL_DEFINE_GUID(IID, LIBID_FlexUDMD, 0x62f0e6a3, 0xd1af, 0x45cf, 0x88,0x93, 0x84,0x6e,0xce,0xb3,0x8d,0xfe);
#ifndef __STANDALONE__
MIDL_DEFINE_GUID(CLSID, CLSID_DMDObject, 0xe1612654, 0x304a, 0x4e07, 0xa2,0x36, 0xeb,0x64,0xd6,0xd4,0xf5,0x11);
#endif

#ifdef __cplusplus
}
#endif

#undef MIDL_DEFINE_GUID
