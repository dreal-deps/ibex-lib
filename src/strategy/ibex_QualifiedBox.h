//============================================================================
//                                  I B E X
// File        : ibex_QualifiedBox.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 21, 2017
// Last update : Mar 15, 2018
//============================================================================

#ifndef __IBEX_SOLVER_OUTPUT_BOX_H__
#define __IBEX_SOLVER_OUTPUT_BOX_H__

#include "ibex_IntervalVector.h"
#include "ibex_VarSet.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Output box of the Solver.
 *
 * Given a system of m equalities f(x)=0 and inequalities g(x)<=0:
 *
 * An output box ([p],[x]) is INNER only if
 *
 *     for all x in [x], for all p in [p]
 *     g(x,p)<=0                                      (1)
 *
 *     for all p in [p] there exists x in [x]
 *     f(x,p)=0                                       (2)
 *
 * When the system is well-constrained (m=n), (2) boils down to:
 *
 *     there exists x in [x], f(x)=0
 *
 * so that the output box is a box containing a solution, according
 * to the usual meaning.
 *
 * An output box ([p],[x]) is BOUNDARY only if (2) holds and the manifold
 * f=0 crosses the inequalities in a "regular" way, the exact definition
 * of "regular" depending on the boundary_test flag of the Solver.
 * See #Solver::boundary_test_strength.
 *
 * When the system is under-constrained, the "solution" box ([x],[p])" may be a
 * large box (compared to eps-min). The "varset" structure indicates which
 * components correspond to x and p. It is NULL in case of well-constrained
 * systems (no parameters) or if m=0 (all parameters).

 * The status is UNKNOWN if the box has been processed (precision eps-min reached)
 * but nothing could be proven.
 *
 * The status is PENDING if the box has not been processed (the solver has been
 * interrupted because of a timeout/memory overflow).
 *
 */
class QualifiedBox {
public:
	/**
	 * \brief Possible status of an output box.
	 *
	 * See above.
	 */
	typedef enum { INNER, BOUNDARY, UNKNOWN, PENDING } sol_status;

	/**
	 * \brief Create a box
	 */
	QualifiedBox(const IntervalVector& box, sol_status status=PENDING, const IntervalVector* unicity=NULL, const VarSet* varset=NULL);

	/**
	 * \brief Duplicate the solution
	 */
	QualifiedBox(const QualifiedBox& sol);

	/**
	 * \brief Assignment
	 */
	QualifiedBox& operator=(const QualifiedBox&);

	/**
	 * \brief Destructor.
	 */
	~QualifiedBox();

	/*
	 * \brief Status of the output box
	 *
	 * See above.
	 */
	const sol_status status;

	/**
	 * \brief Get the solution box.
	 *
	 * Corresponds to the existence box.
	 */
	operator const IntervalVector&() const;

	/**
	 * \brief Existence box.
	 *
	 * If the status is INNER/BOUNDARY, represents the smallest box found
	 * enclosing the manifold.
	 * Otherwise, represents the "unknown" box.
	 */
	const IntervalVector& existence() const;

	/**
	 * \brief Unicity box.
	 *
	 * If the status is INNER/BOUNDARY, represents the largest superset
	 * of existence() found such that the solution enclosed is unique.
	 * Otherwise, represents the "unknown" box.
	 */
	const IntervalVector& unicity() const;

	/**
	 * \brief Variable/Parameter structure.
	 *
	 * Structure used to certify the box.
	 *
	 * This field is NULL in the following cases:
	 *   - the system is well-constrained (all are variables)
	 *   - the system has no equalities (all are parameters)
	 *   - the set of parameters has been fixed by the user
	 *     (same parameters for all solutions)
	 *   - the status is UNKNOWN or PENDING
	 */
	const VarSet* varset;


private:
	IntervalVector _existence;
	IntervalVector* _unicity; // NULL if status=UNKNOWN/PENDING or m=0
};


/**
 * \brief Print the output box
 *
 * Print its status and the parameters/variables structure.
 */
std::ostream& operator<<(std::ostream& os, const QualifiedBox& sol);


/*============================================ inline implementation ============================================ */

inline QualifiedBox::QualifiedBox(const QualifiedBox& sol) : status(sol.status),
		varset(sol.varset? new VarSet(*sol.varset) : NULL), _existence(sol._existence),
		_unicity(sol._unicity? new IntervalVector(*sol._unicity) : NULL) {

}

inline QualifiedBox& QualifiedBox::operator=(const QualifiedBox& sol) {
	(sol_status&) status=sol.status;
	if (varset) delete varset;
	varset=sol.varset? new VarSet(*sol.varset) : NULL;
	_existence=sol._existence;
	if (_unicity) delete _unicity;
	_unicity=sol._unicity? new IntervalVector(*sol._unicity) : NULL;
	return *this;
}

inline QualifiedBox::~QualifiedBox() {
	if (_unicity) delete _unicity;
	if (varset) delete varset;
}

inline QualifiedBox::operator const IntervalVector&() const {
	return existence();
}

inline const IntervalVector& QualifiedBox::existence() const {
	return _existence;
}

inline const IntervalVector& QualifiedBox::unicity() const {
	return _unicity? *_unicity : _existence;
}

} /* namespace ibex */

#endif /* __IBEX_SOLVER_OUTPUT_BOX_H__ */
