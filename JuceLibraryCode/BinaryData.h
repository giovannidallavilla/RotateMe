/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Bright_xml;
    const int            Bright_xmlSize = 228;

    extern const char*   Deep_xml;
    const int            Deep_xmlSize = 228;

    extern const char*   Warm_xml;
    const int            Warm_xmlSize = 228;

    extern const char*   Init_xml;
    const int            Init_xmlSize = 199;

    extern const char*   Jauza_otf;
    const int            Jauza_otfSize = 16248;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
