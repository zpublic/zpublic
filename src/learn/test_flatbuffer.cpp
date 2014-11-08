#include "stdafx.h"
#include "test_flatbuffer.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"

void test_flatbuffer()
{
    ///> 心好累 以后再写吧
//     flatbuffers::FlatBufferBuilder build;
//     auto off = flatbuffers::FieldIndexToOffset(0);
//     build.AddElement<int>(off, 123, 0);
//     off = flatbuffers::FieldIndexToOffset(1);
//     build.AddElement<int>(off, 321, 0);
// 
//     std::string s(reinterpret_cast<const char *>(build.GetBufferPointer()), build.GetSize());
//     flatbuffers::Parser parser;
//     parser.Parse(s.c_str());
//     std::string jsongen;
//     GenerateText(parser, parser.builder_.GetBufferPointer(), 2, &jsongen);
}
