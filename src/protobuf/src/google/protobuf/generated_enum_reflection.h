﻿// protocol buffers - google's data interchange format
// copyright 2008 google inc.  all rights reserved.
// http://code.google.com/p/protobuf/
//
// redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * neither the name of google inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// this software is provided by the copyright holders and contributors
// "as is" and any express or implied warranties, including, but not
// limited to, the implied warranties of merchantability and fitness for
// a particular purpose are disclaimed. in no event shall the copyright
// owner or contributors be liable for any direct, indirect, incidental,
// special, exemplary, or consequential damages (including, but not
// limited to, procurement of substitute goods or services; loss of use,
// data, or profits; or business interruption) however caused and on any
// theory of liability, whether in contract, strict liability, or tort
// (including negligence or otherwise) arising in any way out of the use
// of this software, even if advised of the possibility of such damage.

// author: jasonh@google.com (jason hsueh)
//
// this header is logically internal, but is made public because it is used
// from protocol-compiler-generated code, which may reside in other components.
// it provides reflection support for generated enums, and is included in
// generated .pb.h files and should have minimal dependencies. the methods are
// implemented in generated_message_reflection.cc.

#ifndef google_protobuf_generated_enum_reflection_h__
#define google_protobuf_generated_enum_reflection_h__

#include <string>

namespace google {
namespace protobuf {
  class enumdescriptor;
}  // namespace protobuf

namespace protobuf {

// returns the enumdescriptor for enum type e, which must be a
// proto-declared enum type.  code generated by the protocol compiler
// will include specializations of this template for each enum type declared.
template <typename e>
const enumdescriptor* getenumdescriptor();

namespace internal {

// helper for enumtype_parse functions: try to parse the string 'name' as an
// enum name of the given type, returning true and filling in value on success,
// or returning false and leaving value unchanged on failure.
libprotobuf_export bool parsenamedenum(const enumdescriptor* descriptor,
                    const string& name,
                    int* value);

template<typename enumtype>
bool parsenamedenum(const enumdescriptor* descriptor,
                    const string& name,
                    enumtype* value) {
  int tmp;
  if (!parsenamedenum(descriptor, name, &tmp)) return false;
  *value = static_cast<enumtype>(tmp);
  return true;
}

// just a wrapper around printing the name of a value. the main point of this
// function is not to be inlined, so that you can do this without including
// descriptor.h.
libprotobuf_export const string& nameofenum(const enumdescriptor* descriptor, int value);

}  // namespace internal
}  // namespace protobuf

}  // namespace google
#endif  // google_protobuf_generated_enum_reflection_h__


