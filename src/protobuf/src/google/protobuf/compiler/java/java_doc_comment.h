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

// author: kenton@google.com (kenton varda)
//  based on original protocol buffers design by
//  sanjay ghemawat, jeff dean, and others.

#ifndef google_protobuf_compiler_java_doc_comment_h__
#define google_protobuf_compiler_java_doc_comment_h__

#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
  namespace io {
    class printer;             // printer.h
  }
}

namespace protobuf {
namespace compiler {
namespace java {

void writemessagedoccomment(io::printer* printer, const descriptor* message);
void writefielddoccomment(io::printer* printer, const fielddescriptor* field);
void writeenumdoccomment(io::printer* printer, const enumdescriptor* enum_);
void writeenumvaluedoccomment(io::printer* printer,
                              const enumvaluedescriptor* value);
void writeservicedoccomment(io::printer* printer,
                            const servicedescriptor* service);
void writemethoddoccomment(io::printer* printer,
                           const methoddescriptor* method);

// exposed for testing only.
libprotoc_export string escapejavadoc(const string& input);

}  // namespace java
}  // namespace compiler
}  // namespace protobuf

}  // namespace google
#endif  // google_protobuf_compiler_java_doc_comment_h__

