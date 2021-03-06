/* ============================================================================
 * I B E X - Interval Vector Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_INTERVAL_VECTOR_H__
#define __TEST_INTERVAL_VECTOR_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_IntervalVector.h"
#include "utils.h"

using namespace ibex;

class TestIntervalVector : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestIntervalVector);
	

		CPPUNIT_TEST(cons01);
		CPPUNIT_TEST(cons02);
		CPPUNIT_TEST(cons03);
		CPPUNIT_TEST(cons04);
		CPPUNIT_TEST(cons05);

		CPPUNIT_TEST(set_empty01);

		CPPUNIT_TEST(is_empty01);
		CPPUNIT_TEST(is_empty02);

		CPPUNIT_TEST(resize01);
		CPPUNIT_TEST(resize02);
		CPPUNIT_TEST(resize03);
		CPPUNIT_TEST(resize04);

		CPPUNIT_TEST(subvector01);
		CPPUNIT_TEST(subvector02);
		CPPUNIT_TEST(subvector03);
		CPPUNIT_TEST(subvector04);
		CPPUNIT_TEST(subvector05);
		CPPUNIT_TEST(subvector06);

		CPPUNIT_TEST(cart_prod01);


		CPPUNIT_TEST(inter01);
		CPPUNIT_TEST(inter02);
		CPPUNIT_TEST(inter03);
		CPPUNIT_TEST(inter04);

		CPPUNIT_TEST(hull01);
		CPPUNIT_TEST(hull02);
		CPPUNIT_TEST(hull03);
		CPPUNIT_TEST(hull04);

		CPPUNIT_TEST(eq01);
		CPPUNIT_TEST(eq02);
		CPPUNIT_TEST(eq03);
		CPPUNIT_TEST(eq04);

		CPPUNIT_TEST(mid01);

		CPPUNIT_TEST(is_flat01);
		CPPUNIT_TEST(is_flat02);
		CPPUNIT_TEST(is_flat03);
		CPPUNIT_TEST(is_flat04);
		CPPUNIT_TEST(is_flat05);

		CPPUNIT_TEST(is_unbounded01);
		CPPUNIT_TEST(is_unbounded02);
		CPPUNIT_TEST(is_unbounded03);
		CPPUNIT_TEST(is_unbounded04);

		CPPUNIT_TEST(is_subset01);
		CPPUNIT_TEST(is_subset02);
		CPPUNIT_TEST(is_subset03);
		CPPUNIT_TEST(is_subset04);
		CPPUNIT_TEST(is_subset05);
		CPPUNIT_TEST(is_subset06);
		CPPUNIT_TEST(is_subset07);

		CPPUNIT_TEST(extr_diam_index01);
		CPPUNIT_TEST(extr_diam_index02);
		CPPUNIT_TEST(extr_diam_index03);
		CPPUNIT_TEST(extr_diam_index04);
		CPPUNIT_TEST(extr_diam_index05);
		CPPUNIT_TEST(extr_diam_index06);
		CPPUNIT_TEST(extr_diam_index07);
		CPPUNIT_TEST(extr_diam_index08);
		CPPUNIT_TEST(extr_diam_index09);
		CPPUNIT_TEST(extr_diam_index10);

		CPPUNIT_TEST(sort_indices01);
		CPPUNIT_TEST(sort_indices02);

		CPPUNIT_TEST(perimeter01);
		CPPUNIT_TEST(perimeter02);

		CPPUNIT_TEST(volume01);
		CPPUNIT_TEST(volume02);
		CPPUNIT_TEST(volume03);

		CPPUNIT_TEST(rel_distance01);

		CPPUNIT_TEST(minus01);
		CPPUNIT_TEST(minus02);
		CPPUNIT_TEST(minus03);

		CPPUNIT_TEST(add01);

		CPPUNIT_TEST(sub01);

		CPPUNIT_TEST(compl01);
		CPPUNIT_TEST(compl02);

		CPPUNIT_TEST(diff01);
		CPPUNIT_TEST(diff02);
		CPPUNIT_TEST(diff03);
		CPPUNIT_TEST(diff04);
		CPPUNIT_TEST(diff05);

		CPPUNIT_TEST(random01);
		CPPUNIT_TEST(random02);
	CPPUNIT_TEST_SUITE_END();

	/* test:
	 * =======
	 *   IntervalVector(int n)
	 *   IntervalVector(const IntervalVector& x)
	 *   operator[](int i) const
	 *   operator=(const IntervalVector& x)
	 *   size() const
	 */
	void cons01();
	// test: IntervalVector(int dim, const Interval& x)
	void cons02();
	// test: IntervalVector(const IntervalVector& x)
	void cons03();
	// test: IntervalVector(int n, double  bounds[][2])
	void cons04();
	// test: empty(int n)
	void cons05();
	// test: set_empty()
	void set_empty01();

	// test: is_empty() const
	void is_empty01();
	void is_empty02();

	// test: resize(int)
	void resize01();
	void resize02();
	void resize03();
	void resize04();

	// test: subvector(int start_index, int end_index)
	void subvector01();
	void subvector02();
	void subvector03();
	void subvector04();
	void subvector05();
	void subvector06();

	void cart_prod01();

	// test:
	//   operator&=(const IntervalVector& x)
	//   operator&(const IntervalVector& x) const
	void inter01();
	void inter02();
	void inter03();
	void inter04();

	// test:
	//   operator|=(const IntervalVector| x)
	//   operator|(const IntervalVector| x) const
	void hull01();
	void hull02();
	void hull03();
	void hull04();

	// test:
	//    operator==(const IntervalVector& x) const
	//    operator!=(const IntervalVector& x) const
	void eq01();
	void eq02();
	void eq03();
	void eq04();

	// test: mid() const
	void mid01();

	// test: is_flat() const
	void is_flat01();
	void is_flat02();
	void is_flat03();
	void is_flat04();
	void is_flat05();

	// test: is_unbounded() const
	void is_unbounded01();
	void is_unbounded02();
	void is_unbounded03();
	void is_unbounded04();

	// test:
	//  is_subset(const IntervalVector& x) const
	//  is_strict_interior_subset(const IntervalVector& x) const
	//  is_superset(const IntervalVector& x) cons
	//  is_strict_superset(const IntervalVector& x) const
	void is_subset01();
	void is_subset02();
	void is_subset03();
	void is_subset04();
	void is_subset05();
	void is_subset06();
	void is_subset07();

	// test:
	//    extr_diam_index() const
	//    min_diam() const
	//    max_diam() const
	void extr_diam_index01();
	void extr_diam_index02();
	void extr_diam_index03();
	void extr_diam_index04();
	void extr_diam_index05();
	void extr_diam_index06();
	void extr_diam_index07();
	void extr_diam_index08();
	void extr_diam_index09();
	void extr_diam_index10();

	void sort_indices01();
	void sort_indices02();

	// test: perimeter() const
	void perimeter01();
	void perimeter02();

	// test: volume() const
	void volume01();
	void volume02();
	void volume03();

	// test: rel_distance() const
	void rel_distance01();

	// test: operator-() const
	void minus01();
	void minus02();
	void minus03();

	// test:
	//  operator+(const IntervalVector& x) const
	//  operator+=(const IntervalVector& x)
	void add01();

	// test:
	//  operator-(const IntervalVector& x) const
	//  operator-=(const IntervalVector& x)
	void sub01();

	// test: complementary(IntervalVector*& result) const
	void compl01();
	void compl02();

	// test: diff(const IntervalVector& y, IntervalVector*& result) const
	void diff01();
	void diff02();
	void diff03();
	void diff04();
	void diff05();

	// test: random() const
	void random01();
	void random02();

private:

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestIntervalVector);


#endif // __TEST_INTERVAL_VECTOR_H__
