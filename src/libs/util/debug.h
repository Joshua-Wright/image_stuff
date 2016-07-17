// (c) Copyright 2016 Josh Wright
// helpful snippets for debugging C++ programs
// These abuse macros, templates and operator overloads
// to make them easier to use
#pragma once

#include <iostream> // for std::cerr and std::endl
#include <vector>
#include <typeinfo> // for typeid(T).name()
#include <cstring> // for strlen() and basename()
#include <cxxabi.h> // for abi::__cxa_demangle()
#include <sstream>
#include <iomanip>

namespace __hidden__ {

    using std::endl;
    using std::setw;

//    auto &out = std::cerr;
#define out std::cerr

    struct print {
        bool space;
        const char *expr;
        const char *file;
        int line;

        print(const char *file, int line, const char *expr) : space(false), expr(expr), file(file), line(line) { }

        ~print() { out << std::endl; }

        template<typename T>
        print &operator,(const T &t) {
            if (space) {
                out << ' ';
            } else {
                out << basename(file) << ":" << line << " " << expr << " = ";
                space = true;
            }
            out << t;
            return *this;
        }
    };

    template<typename T>
    const char *demangle_type_name() {
        /*gcc-specific way to de-mangle the type names, probably not portable*/
        if (typeid(T) == typeid(std::string)) {
            return "std::string";
        } else {
            return abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
        }
    }

    template<typename T>
    void __debug_log(T v, const char *l, const char *f, int line, bool p) {
        /*debug logger that uses template type resolution to print whatever we give it*/
        out << basename(f) << ":" << line << " ";
        if (p) {
            out << demangle_type_name<T>() << " ";
        }
        out << l << "=" << v << endl;
    }

    struct key_value_printer {
        struct line {
            std::string type;
            std::string key;
            std::string value;
        };
        std::vector<line> lines;
        const char *expr;
        const char *file;
        int line;

        key_value_printer(const char *file, int line) : file(file), line(line) { }

        template<typename T>
        key_value_printer &a(const std::string &key, const T &value) {
            std::stringstream val;
            val << value;
            lines.push_back({demangle_type_name<T>(), key, val.str()});
            return *this;
        }

        ~key_value_printer() {
            size_t max_name_length = 0;
            size_t max_type_length = 0;
            for (auto &l : lines) {
                if (l.key.length() > max_name_length) {
                    max_name_length = (int) l.key.length();
                }
                if (l.type.length() > max_type_length) {
                    max_type_length = (int) l.type.length();
                }
            }
            out << basename(file) << ":" << line << ":" << endl;
            for (auto &l : lines) {
                out <<
                setw(max_type_length) << std::left << l.type << " : " <<
                setw(max_name_length) << std::left << l.key << " = "
                << l.value << endl;
            }
        }

    };
}

#define __KV_0 __hidden__::key_value_printer(__FILE__, __LINE__)
/* the python source to generate this macro trees is included in long_macros.py */

#define ARG(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, ...) _31
#define NARG(...) ARG(__VA_ARGS__, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __KV_1(a1) \
	__KV_0.a(#a1,a1)
#define __KV_2(a1,a2) \
	__KV_0.a(#a1,a1).a(#a2,a2)
#define __KV_3(a1,a2,a3) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3)
#define __KV_4(a1,a2,a3,a4) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4)
#define __KV_5(a1,a2,a3,a4,a5) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5)
#define __KV_6(a1,a2,a3,a4,a5,a6) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6)
#define __KV_7(a1,a2,a3,a4,a5,a6,a7) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7)
#define __KV_8(a1,a2,a3,a4,a5,a6,a7,a8) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8)
#define __KV_9(a1,a2,a3,a4,a5,a6,a7,a8,a9) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9)
#define __KV_10(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10)
#define __KV_11(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11)
#define __KV_12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12)
#define __KV_13(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13)
#define __KV_14(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14)
#define __KV_15(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15)
#define __KV_16(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16)
#define __KV_17(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17)
#define __KV_18(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18)
#define __KV_19(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19)
#define __KV_20(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20)
#define __KV_21(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21)
#define __KV_22(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22)
#define __KV_23(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23)
#define __KV_24(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24)
#define __KV_25(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25)
#define __KV_26(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25).a(#a26,a26)
#define __KV_27(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25).a(#a26,a26).a(#a27,a27)
#define __KV_28(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25).a(#a26,a26).a(#a27,a27).a(#a28,a28)
#define __KV_29(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25).a(#a26,a26).a(#a27,a27).a(#a28,a28).a(#a29,a29)
#define __KV_30(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25).a(#a26,a26).a(#a27,a27).a(#a28,a28).a(#a29,a29).a(#a30,a30)
#define __KV_31(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25).a(#a26,a26).a(#a27,a27).a(#a28,a28).a(#a29,a29).a(#a30,a30).a(#a31,a31)
#define __KV_32(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32) \
	__KV_0.a(#a1,a1).a(#a2,a2).a(#a3,a3).a(#a4,a4).a(#a5,a5).a(#a6,a6).a(#a7,a7).a(#a8,a8).a(#a9,a9).a(#a10,a10).a(#a11,a11).a(#a12,a12).a(#a13,a13).a(#a14,a14).a(#a15,a15).a(#a16,a16).a(#a17,a17).a(#a18,a18).a(#a19,a19).a(#a20,a20).a(#a21,a21).a(#a22,a22).a(#a23,a23).a(#a24,a24).a(#a25,a25).a(#a26,a26).a(#a27,a27).a(#a28,a28).a(#a29,a29).a(#a30,a30).a(#a31,a31).a(#a32,a32)


#define ARG_MULTIPLE(N, ...) __KV_ ## N (__VA_ARGS__)
// the following is necessary to make sure the processor has a chance to evaluate NARG16
#define _ARG_MULTIPLE(N, ...) ARG_MULTIPLE(N, __VA_ARGS__)
#define KV(...) _ARG_MULTIPLE(NARG(__VA_ARGS__), __VA_ARGS__)


#define DEBUG_LOG(x) __hidden__::__debug_log(x, #x, __FILE__, __LINE__, true)
#define DEBUG_LOG_TYPE(x) __hidden__::__debug_log(x, #x, __FILE__, __LINE__, true)
#define DEBUG_LOG_NOTYPE(x) __hidden__::__debug_log(x, #x, __FILE__, __LINE__, false)

#define DEBUG_PRINT(...) __hidden__::print(__FILE__, __LINE__, (#__VA_ARGS__)),  __VA_ARGS__;


