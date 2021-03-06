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

#ifndef google_protobuf_compiler_java_enum_field_h__
#define google_protobuf_compiler_java_enum_field_h__

#include <map>
#include <string>
#include <google/protobuf/compiler/java/java_field.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace java {

class enumfieldgenerator : public fieldgenerator {
 public:
  explicit enumfieldgenerator(const fielddescriptor* descriptor,
      int messagebitindex, int builderbitindex);
  ~enumfieldgenerator();

  // implements fieldgenerator ---------------------------------------
  int getnumbitsformessage() const;
  int getnumbitsforbuilder() const;
  void generateinterfacemembers(io::printer* printer) const;
  void generatemembers(io::printer* printer) const;
  void generatebuildermembers(io::printer* printer) const;
  void generateinitializationcode(io::printer* printer) const;
  void generatebuilderclearcode(io::printer* printer) const;
  void generatemergingcode(io::printer* printer) const;
  void generatebuildingcode(io::printer* printer) const;
  void generateparsingcode(io::printer* printer) const;
  void generateparsingdonecode(io::printer* printer) const;
  void generateserializationcode(io::printer* printer) const;
  void generateserializedsizecode(io::printer* printer) const;
  void generatefieldbuilderinitializationcode(io::printer* printer) const;
  void generateequalscode(io::printer* printer) const;
  void generatehashcode(io::printer* printer) const;

  string getboxedtype() const;

 private:
  const fielddescriptor* descriptor_;
  map<string, string> variables_;
  const int messagebitindex_;
  const int builderbitindex_;

  google_disallow_evil_constructors(enumfieldgenerator);
};

class repeatedenumfieldgenerator : public fieldgenerator {
 public:
  explicit repeatedenumfieldgenerator(const fielddescriptor* descriptor,
      int messagebitindex, int builderbitindex);
  ~repeatedenumfieldgenerator();

  // implements fieldgenerator ---------------------------------------
  int getnumbitsformessage() const;
  int getnumbitsforbuilder() const;
  void generateinterfacemembers(io::printer* printer) const;
  void generatemembers(io::printer* printer) const;
  void generatebuildermembers(io::printer* printer) const;
  void generateinitializationcode(io::printer* printer) const;
  void generatebuilderclearcode(io::printer* printer) const;
  void generatemergingcode(io::printer* printer) const;
  void generatebuildingcode(io::printer* printer) const;
  void generateparsingcode(io::printer* printer) const;
  void generateparsingcodefrompacked(io::printer* printer) const;
  void generateparsingdonecode(io::printer* printer) const;
  void generateserializationcode(io::printer* printer) const;
  void generateserializedsizecode(io::printer* printer) const;
  void generatefieldbuilderinitializationcode(io::printer* printer) const;
  void generateequalscode(io::printer* printer) const;
  void generatehashcode(io::printer* printer) const;

  string getboxedtype() const;

 private:
  const fielddescriptor* descriptor_;
  map<string, string> variables_;
  const int messagebitindex_;
  const int builderbitindex_;

  google_disallow_evil_constructors(repeatedenumfieldgenerator);
};

}  // namespace java
}  // namespace compiler
}  // namespace protobuf

}  // namespace google
#endif  // google_protobuf_compiler_java_enum_field_h__


