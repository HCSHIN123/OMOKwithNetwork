// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: enum.proto

#include "enum.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace Protocol {
}  // namespace Protocol
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_enum_2eproto[2];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_enum_2eproto = nullptr;
const uint32_t TableStruct_enum_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_enum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\nenum.proto\022\010Protocol*-\n\013STONE_COLOR\022\t\n"
  "\005BLACK\020\000\022\t\n\005WHITE\020\001\022\010\n\004NONE\020\002*(\n\tGAME_OV"
  "ER\022\007\n\003WIN\020\000\022\010\n\004LOSE\020\001\022\010\n\004EXIT\020\002b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_enum_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_enum_2eproto = {
    false, false, 119, descriptor_table_protodef_enum_2eproto,
    "enum.proto",
    &descriptor_table_enum_2eproto_once, nullptr, 0, 0,
    schemas, file_default_instances, TableStruct_enum_2eproto::offsets,
    nullptr, file_level_enum_descriptors_enum_2eproto,
    file_level_service_descriptors_enum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_enum_2eproto_getter() {
  return &descriptor_table_enum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_enum_2eproto(&descriptor_table_enum_2eproto);
namespace Protocol {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* STONE_COLOR_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_enum_2eproto);
  return file_level_enum_descriptors_enum_2eproto[0];
}
bool STONE_COLOR_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* GAME_OVER_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_enum_2eproto);
  return file_level_enum_descriptors_enum_2eproto[1];
}
bool GAME_OVER_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
