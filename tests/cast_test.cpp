/* cast_test.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 28 Dec 2015
   FreeBSD-style copyright and disclaimer apply
*/

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "reflect.h"
#include "tests.h"
#include "test_types.h"

#include <boost/test/unit_test.hpp>

using namespace test;
using namespace reflect;

template<typename T>
Value lref(T&& val) { return Value(std::forward<T>(val)); }

template<typename T>
Value clref(T&& val) { return Value(std::forward<T>(val)).toConst(); }

template<typename T>
Value rref(T&& val) { return Value(std::forward<T>(val)).rvalue(); }


/******************************************************************************/
/* COPY                                                                       */
/******************************************************************************/

BOOST_AUTO_TEST_CASE(test_copy)
{
    BOOST_CHECK_EQUAL(cast<int>(rref(10)), 10);

    {
        int val = 20;
        auto result = cast<int>(lref(val));
        BOOST_CHECK_EQUAL(result, val);
        BOOST_CHECK_NE(&result, &val);
    }

    {
        const int cval = 30;
        auto result = cast<int>(lref(cval));
        BOOST_CHECK_EQUAL(result, cval);
        BOOST_CHECK_NE(&result, &cval);
    }

    BOOST_CHECK_EQUAL(cast<Object>(rref(Object(40))), Object(40));

    {
        Object obj(50);
        auto result = cast<Object>(lref(obj));
        BOOST_CHECK_EQUAL(result, obj);
        BOOST_CHECK_NE(&result, &obj);
    }

    {
        const Object cobj(60);
        auto result = cast<Object>(lref(cobj));
        BOOST_CHECK_EQUAL(result, cobj);
        BOOST_CHECK_NE(&result, &cobj);
    }

    CHECK_ERROR(cast<NotCopiable>(rref(NotCopiable())));

    {
        NotCopiable noCopy;
        CHECK_ERROR(cast<NotCopiable>(lref(noCopy)));
    }

    BOOST_CHECK_EQUAL(cast<Parent>(rref(Child(70))), Parent(70));

    {
        Child child(70);
        auto result = cast<Parent>(lref(child));
        BOOST_CHECK_EQUAL(result.value, 70);
        BOOST_CHECK_NE(&result, &child);
    }

    {
        const Child cchild(90);
        auto result = cast<Parent>(lref(cchild));
        BOOST_CHECK_EQUAL(result.value, 90);
        BOOST_CHECK_NE(&result, &cchild);
    }

    BOOST_CHECK_EQUAL(cast<int>(rref(Convertible(90))), 90);
    BOOST_CHECK_EQUAL(cast<int>(lref(Convertible(100))), 100);

    {
        const Convertible cconv(110);
        BOOST_CHECK_EQUAL(cast<int>(lref(cconv)), 110);
    }
}


/******************************************************************************/
/* lref                                                                       */
/******************************************************************************/

BOOST_AUTO_TEST_CASE(test_lref)
{

    int iVal = 10;
    CHECK_ERROR(cast<int&>(rref(int(10))));
    BOOST_CHECK_EQUAL(&cast<int&>(lref(iVal)), &iVal);
    CHECK_ERROR(cast<int&>(clref(iVal)));

    Object objVal(50);
    CHECK_ERROR(cast<Object&>(rref(Object(40))));
    BOOST_CHECK_EQUAL(&cast<Object&>(lref(objVal)), &objVal);
    CHECK_ERROR(cast<Object&>(clref(objVal)));

    NotCopiable ncVal;
    CHECK_ERROR(cast<NotCopiable&>(rref(NotCopiable())));
    BOOST_CHECK_EQUAL(&cast<NotCopiable&>(lref(ncVal)), &ncVal);

    Child childVal(70);
    CHECK_ERROR(cast<Parent&>(rref(Child(70))));
    BOOST_CHECK_EQUAL(&cast<Parent&>(lref(childVal)), &childVal);
    CHECK_ERROR(cast<Parent&>(clref(childVal)));

    CHECK_ERROR(cast<int&>(rref(Convertible(90))));
    CHECK_ERROR(cast<int&>(lref(Convertible(100))));
    CHECK_ERROR(cast<int&>(clref(Convertible(110))));
}


/******************************************************************************/
/* clref                                                                      */
/******************************************************************************/

BOOST_AUTO_TEST_CASE(test_clref)
{
    int iVal = 10;
    BOOST_CHECK_EQUAL(cast<const int&>(rref(iVal)), iVal);
    BOOST_CHECK_EQUAL(&cast<const int&>(lref(iVal)), &iVal);
    BOOST_CHECK_EQUAL(&cast<const int&>(clref(iVal)), &iVal);

    Object objVal(40);
    BOOST_CHECK_EQUAL(cast<const Object&>(rref(objVal)), Object(40));
    BOOST_CHECK_EQUAL(&cast<const Object&>(lref(objVal)), &objVal);
    BOOST_CHECK_EQUAL(&cast<const Object&>(clref(objVal)), &objVal);

    NotCopiable ncVal;
    cast<const NotCopiable&>(rref(NotCopiable()));
    BOOST_CHECK_EQUAL(&cast<const NotCopiable&>(lref(ncVal)), &ncVal);
    BOOST_CHECK_EQUAL(&cast<const NotCopiable&>(clref(ncVal)), &ncVal);

    Child childVal(70);
    BOOST_CHECK_EQUAL(cast<const Parent&>(rref(Child(70))), Parent(70));
    BOOST_CHECK_EQUAL(&cast<const Parent&>(lref(childVal)), &childVal);
    BOOST_CHECK_EQUAL(&cast<const Parent&>(clref(childVal)), &childVal);

    Convertible convVal(90);
    CHECK_ERROR(cast<const int&>(rref(Convertible(90))));
    CHECK_ERROR(cast<const int&>(lref(Convertible(100))));
    CHECK_ERROR(cast<const int&>(clref(Convertible(110))));
}


/******************************************************************************/
/* rref                                                                       */
/******************************************************************************/

// Note that we allow lref to rref casts because Value can only represent lref
// and rrefs and not copy which get classified as lrefs. This is a quality of
// life thing for the interface.
BOOST_AUTO_TEST_CASE(test_rref)
{
    BOOST_CHECK_EQUAL(cast<int&&>(rref(int(10))), 10);
    {
        int val = 20;
        BOOST_CHECK(!isCastable<int&&>(lref(val)));
        BOOST_CHECK_EQUAL(cast<int&&>(lref(val)), 20);
    }
    {
        int val = 30;
        BOOST_CHECK(!isCastable<int&&>(clref(val)));
        BOOST_CHECK_EQUAL(cast<int&&>(clref(val)), 30);
    }

    BOOST_CHECK_EQUAL(cast<Object&&>(rref(Object(40))), Object(40));
    {
        Object val(50);
        BOOST_CHECK(!isCastable<Object&&>(lref(val)));
        BOOST_CHECK_EQUAL(cast<Object&&>(lref(val)), Object(50));
    }
    {
        Object val(60);
        BOOST_CHECK(!isCastable<Object&&>(clref(val)));
        BOOST_CHECK_EQUAL(cast<Object&&>(clref(val)), Object(60));
    }

    NotCopiable ncVal;
    cast<NotCopiable&&>(rref(NotCopiable()));
    cast<NotCopiable&&>(lref(ncVal));
    cast<NotCopiable&&>(clref(ncVal));

    BOOST_CHECK_EQUAL(cast<Parent&&>(rref(Child(70))), Parent(70));
    {
        Child val(70);
        BOOST_CHECK(!isCastable<Parent&&>(lref(val)));
        BOOST_CHECK_EQUAL(cast<Parent&&>(lref(val)), Parent(70));
    }
    {
        Child val(80);
        BOOST_CHECK(!isCastable<Parent&&>(clref(val)));
        BOOST_CHECK_EQUAL(cast<Parent&&>(clref(val)), Parent(80));
    }

    Convertible convVal(90);
    BOOST_CHECK_EQUAL(cast<int&&>(rref(Convertible(90))), 90);
    BOOST_CHECK_EQUAL(cast<int&&>(lref(Convertible(100))), 100);
    BOOST_CHECK_EQUAL(cast<int&&>(clref(Convertible(110))), 110);
}
