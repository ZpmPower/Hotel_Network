// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MessageType.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "MessageType.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace network {

namespace {

const ::google::protobuf::EnumDescriptor* message_type_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_MessageType_2eproto() {
  protobuf_AddDesc_MessageType_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "MessageType.proto");
  GOOGLE_CHECK(file != NULL);
  message_type_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_MessageType_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_MessageType_2eproto() {
}

void protobuf_AddDesc_MessageType_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021MessageType.proto\022\007network*\375\004\n\014message"
    "_type\022\017\n\013HN_REGISTER\020\000\022\013\n\007HN_AUTH\020\001\022\025\n\021H"
    "N_GET_ALL_GUESTS\020\002\022\030\n\024HN_REGISTER_EMPLOY"
    "EE\020\003\022\030\n\024HN_GET_ALL_EMPLOYEES\020\004\022\025\n\021HN_GET"
    "_ALL_HOTELS\020\005\022\024\n\020HN_GET_ALL_ROOMS\020\006\022\032\n\026H"
    "N_GET_HOTEL_EMPLOYEES\020\007\022\024\n\020HN_EDIT_EMPLO"
    "YEE\020\010\022\026\n\022HN_DELETE_EMPLOYEE\020\t\022\026\n\022HN_GET_"
    "HOTEL_ROOMS\020\n\022\022\n\016HN_DELETE_ROOM\020\013\022\025\n\021HN_"
    "GET_ROOM_TYPES\020\014\022\025\n\021HN_ADD_HOTEL_ROOM\020\r\022"
    "\026\n\022HN_EDIT_HOTEL_ROOM\020\016\022\027\n\023HN_GET_HOTEL_"
    "ORDERS\020\017\022\026\n\022HN_GET_HOTEL_TYPES\020\020\022\027\n\023HN_G"
    "ET_VACANT_ROOMS\020\021\022\021\n\rHN_MAKE_ORDER\020\022\022\031\n\025"
    "HN_GET_CURRENT_GUESTS\020\023\022\030\n\024HN_COUNT_HOTE"
    "L_ROOMS\020\024\022\034\n\030HN_COUNT_HOTEL_EMPLOYEES\020\025\022"
    "\031\n\025HN_COUNT_HOTEL_ORDERS\020\026\022\031\n\025HN_AVG_RES"
    "IDENCE_TIME\020\027\022\026\n\022HN_AVG_ROOM_RATING\020\030\022\035\n"
    "\031HN_GET_VACANT_ROOMS_GUEST\020\031", 668);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "MessageType.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_MessageType_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_MessageType_2eproto {
  StaticDescriptorInitializer_MessageType_2eproto() {
    protobuf_AddDesc_MessageType_2eproto();
  }
} static_descriptor_initializer_MessageType_2eproto_;
const ::google::protobuf::EnumDescriptor* message_type_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return message_type_descriptor_;
}
bool message_type_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace network

// @@protoc_insertion_point(global_scope)