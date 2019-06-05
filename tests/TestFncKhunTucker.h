/* ============================================================================
 * I B E X - TestFncKhunTucker
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 17, 2019
 * Last Update : May 17, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FNC_KHUN_TUCKER_H__
#define __TEST_FNC_KHUN_TUCKER_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <fstream>

#include "utils.h"

#include "ibex_IntervalVector.h"
#include "ibex_Cov.h"
#include "ibex_CovList.h"
#include "ibex_CovIUList.h"
#include "ibex_CovIBUList.h"
#include "ibex_CovManifold.h"
#include "ibex_CovSolverData.h"
#include "ibex_Solver.h"

class TestFncKhunTucker : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestFncKhunTucker);

	CPPUNIT_TEST(one_var);
	CPPUNIT_TEST(one_var_1_bound);
	CPPUNIT_TEST(one_var_1_ineq_1_bound);
	CPPUNIT_TEST_SUITE_END();

private:
	void one_var();
	void one_var_1_bound();
	void one_var_1_ineq_1_bound();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFncKhunTucker);

#endif // __TEST_FNC_KHUN_TUCKER__
