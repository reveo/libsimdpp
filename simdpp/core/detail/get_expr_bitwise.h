/*  Copyright (C) 2014  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_CORE_DETAIL_GET_EXPR_BITWISE_H
#define LIBSIMDPP_SIMDPP_CORE_DETAIL_GET_EXPR_BITWISE_H

#include <simdpp/detail/get_expr.h>

namespace simdpp {
#ifndef SIMDPP_DOXYGEN
namespace SIMDPP_ARCH_NAMESPACE {
#endif
namespace detail {


/*  We want to reduce the number of overloads that need to be created in order
    to match a specific case of an expression tree containing 'bit_and',
    or 'bit_andnot'. nodes. The following "optimizations"
    are performed:

      * If the parameters are types have different element sizes then both
        expression arguments have the same type as the expression itself, except
        that signed vectors are converted to unsigned vectors.
      * Otherwise if the expression is of a mask type then both types are the
        same as the expression itself.
      * Otherwise both types have the same type as the expression itself, except
        that signed vectors are converted to unsigned vectors and if the second
        type is a mask type then it is converted to floating-point mask if the
        expression is floating-point expression and to integer mask otherwise.

     So, as a result, the following tuples of types will appear as the arguments
     of the returned expression:

      * mask_int8, mask_int8
      * uint8, mask_int8
      * uint8, uint8
      * mask_int16, mask_int16
      * uint16, mask_int16
      * uint16, uint16
      * mask_int32, mask_int32
      * uint32, mask_int32
      * uint32, uint32
      * mask_int64, mask_int64
      * uint64, mask_int64
      * uint64, uint64
      * mask_float32, mask_float32
      * float32, mask_float32
      * float32, float32
      * mask_float64, mask_float64
      * float64, mask_float64
      * float64, float64

    The type of the returned expression is governed by the usual rules
    (see simdpp/types/tag.h)
*/

template<template<class, class> class E, class V1, class V2>
class get_expr_bitwise2_and {

    // (size_tag) get the size tag of the resulting expression
    static const unsigned size_tag = V1::size_tag > V2::size_tag ? V1::size_tag : V2::size_tag;

    // (type_tag) get the type tag of the expression. We compute it in the same
    // way get_expr2 computes them, i.e.
    // type_tag == get_expr2<V1,V2,void>::type::type_tag
    static const unsigned type_tag_t1 = V1::type_tag > V2::type_tag ? V1::type_tag : V2::type_tag;
    static const bool is_mask_op1 = type_tag_t1 == SIMDPP_TAG_MASK_INT ||
                                    type_tag_t1 == SIMDPP_TAG_MASK_FLOAT;
    static const unsigned type_tag = (is_mask_op1 && V1::size_tag != V2::size_tag)
                                    ? SIMDPP_TAG_UINT : type_tag_t1;

    // strip signed integer types
    static const unsigned v1_type_tag = type_tag == SIMDPP_TAG_INT ? SIMDPP_TAG_UINT : type_tag;


    static const bool is_v2_mask = V2::type_tag == SIMDPP_TAG_MASK_INT ||
                                   V2::type_tag == SIMDPP_TAG_MASK_FLOAT;
    static const bool is_v1_float = type_tag == SIMDPP_TAG_FLOAT ||
                                     type_tag == SIMDPP_TAG_MASK_FLOAT;

    // if second parameter is a mask then convert the mask to float mask if the
    // expression is float and to integer mask otherwise
    static const unsigned v2_type_tag = (!is_v2_mask) ? v1_type_tag :
                                        is_v1_float ? SIMDPP_TAG_MASK_FLOAT :
                                        SIMDPP_TAG_MASK_INT;

public:
    using v1_type = typename type_of_tag<v1_type_tag + size_tag, V1::length_bytes, void>::type;
    using v2_type = typename type_of_tag<v2_type_tag + size_tag, V1::length_bytes, void>::type;

    using type = typename type_of_tag<type_tag + size_tag, V1::length_bytes,
                                      E<v1_type, v2_type>>::type;
};


/*  The case with bit_or is similar to bit_and and bit_andnot except that the
    expression types are either leave two masks or none.

      * Both expression arguments have the same type as the expression itself,
        except that signed vectors are converted to unsigned vectors.

     So, as a result, the following tuples of types will appear as the arguments
     of the returned expression:

      * mask_int8, mask_int8
      * uint8, uint8
      * mask_int16, mask_int16
      * uint16, uint16
      * mask_int32, mask_int32
      * uint32, uint32
      * mask_int64, mask_int64
      * uint64, uint64
      * mask_float32, mask_float32
      * float32, float32
      * mask_float64, mask_float64
      * float64, float64

    The type of the returned expression is governed by the usual rules
    (see simdpp/types/tag.h)
*/

template<class V1, class V2>
class get_expr_bit_or {

    // (size_tag) get the size tag of the resulting expression
    static const unsigned size_tag = V1::size_tag > V2::size_tag ? V1::size_tag : V2::size_tag;

    // (type_tag) get the type tag of the expression. We compute it in the same
    // way get_expr2 computes them, i.e.
    // type_tag == get_expr2<V1,V2,void>::type::type_tag
    static const unsigned type_tag_t1 = V1::type_tag > V2::type_tag ? V1::type_tag : V2::type_tag;
    static const bool is_mask_op1 = type_tag_t1 == SIMDPP_TAG_MASK_INT ||
                                    type_tag_t1 == SIMDPP_TAG_MASK_FLOAT;
    static const unsigned type_tag = (is_mask_op1 && V1::size_tag != V2::size_tag)
                                    ? SIMDPP_TAG_UINT : type_tag_t1;

    // strip signed integer types
    static const unsigned v12_type_tag = type_tag == SIMDPP_TAG_INT ? SIMDPP_TAG_UINT : type_tag;


public:
    using v12_type = typename type_of_tag<v12_type_tag + size_tag, V1::length_bytes, void>::type;

    using type = typename type_of_tag<type_tag + size_tag, V1::length_bytes,
                                      expr_bit_or<v12_type, v12_type>>::type;
};



} // namespace detail
#ifndef SIMDPP_DOXYGEN
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif