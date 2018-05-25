// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MessageType.proto

#ifndef PROTOBUF_MessageType_2eproto__INCLUDED
#define PROTOBUF_MessageType_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace network {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_MessageType_2eproto();
void protobuf_AssignDesc_MessageType_2eproto();
void protobuf_ShutdownFile_MessageType_2eproto();


enum message_type {
  HN_REGISTER = 0,
  HN_AUTH = 1
};
bool message_type_IsValid(int value);
const message_type message_type_MIN = HN_REGISTER;
const message_type message_type_MAX = HN_AUTH;
const int message_type_ARRAYSIZE = message_type_MAX + 1;

const ::google::protobuf::EnumDescriptor* message_type_descriptor();
inline const ::std::string& message_type_Name(message_type value) {
  return ::google::protobuf::internal::NameOfEnum(
    message_type_descriptor(), value);
}
inline bool message_type_Parse(
    const ::std::string& name, message_type* value) {
  return ::google::protobuf::internal::ParseNamedEnum<message_type>(
    message_type_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace network

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::network::message_type> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::network::message_type>() {
  return ::network::message_type_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_MessageType_2eproto__INCLUDED
