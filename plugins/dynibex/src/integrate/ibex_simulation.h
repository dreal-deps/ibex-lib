/* ============================================================================
 * D Y N I B E X - Definition of the Simulation
 * ============================================================================
 * Copyright   : ENSTA ParisTech
 * License     : This program can be distributed under the terms of the GNU
 * LGPL. See the file COPYING.LESSER.
 *
 * Author(s)   : Julien Alexandre dit Sandretto and Alexandre Chapoutot
 * Created     : Jul 18, 2014
 * Modified    : Oct, 2015 (addition of DAE)
 * Sponsored   : This research benefited from the support of the "Chair Complex
 * Systems Engineering - Ecole Polytechnique, THALES, DGA, FX, DASSAULT
 * AVIATION, DCNS Research, ENSTA ParisTech, Telecom ParisTech, Fondation
 * ParisTech and FDO ENSTA"
 * ----------------------------------------------------------------------------
 */
#ifndef IBEX_SIMU_H
#define IBEX_SIMU_H

#include "ibex_integrate.h"
#include "ibex_solution_HEUN.h"
#include "ibex_solution_LA3.h"
#include "ibex_solution_LC3.h"
#include "ibex_solution_RADAU3.h"
#include "ibex_solution_RK4.h"
#include "ibex_solution_TAYLOR4.h"
#include "ibex_solution_iEULER.h"
#include "ibex_solution_iMIDPOINT.h"

#include <fstream>
#include <stdlib.h>

namespace ibex {
// namespace std{

enum Method {
  IEULER,
  IMIDPOINT,
  RADAU3,
  HEUN,
  TAYLOR4,
  LA3,
  LC3,
  RK4,
  RADAU3_DAE
};

class simulation {
public:
  double time_T;
  int process;
  double atol;
  int test;
  Method meth;
  int nb_var;

  bool monotony_active;

  // ode
  std::list<solution_j> list_solution_j;
  ivp_ode *embedded_ode;
  // dae
  std::list<solution_j_dae> list_solution_j_dae;
  ivp_dae_h1 *embedded_dae;

  // test
  std::list<solution_g> list_solution_g;

  // var for monotony use
  void active_monotony() { monotony_active = true; }

  void inactive_monotony() { monotony_active = false; }

  // fill genericfrom whatever solution (ode or dae)
  void sol_to_generic(const std::list<solution_j> &list_sol) {
    for (const auto &item : list_sol) {
      list_solution_g.emplace_back(item.time_j, item.box_j1, item.box_jn,
                                   item.box_jnh, nb_var);
    }
  }

  void sol_to_generic(const std::list<solution_j_dae> &list_sol) {
    for (const auto &item : list_sol) {
      list_solution_g.emplace_back(item.time_j, item.box_j1, item.box_jn,
                                   item.box_jnh, nb_var);
    }
  }

  // compute solutions after solutions till t >= time_T
  int run_simulation() {
    double tn;
    solution_j sol_temp;
    solution_j_dae sol_temp_dae;
    double fac;
    double newh;
    do {
      // reuse of the first (and verified step) to build the followers
      if (!list_solution_j.empty()) {
        sol_temp = list_solution_j.back();
        // initialize next solution
        tn = sol_temp.time_j.ub();
        fac = sol_temp.factor;
        newh = std::abs(sol_temp.time_j.diam()) *
               fac; // we apply the factor estimated the last step
      } else {
        sol_temp_dae = list_solution_j_dae.back();
        // initialize next solution
        tn = sol_temp_dae.time_j.ub();
        fac = sol_temp_dae.factor;
        newh = std::abs(sol_temp_dae.time_j.diam()) *
               fac; // we apply the factor estimated the last step
      }

      if (tn < time_T) {
        double delta_t = time_T - tn;

        if (delta_t < newh)
          newh = delta_t;

        switch (meth) {
        case IEULER: {
          solution_j_ieuler u_j(*sol_temp.box_jnh_aff, tn, newh, embedded_ode,
                                atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case IMIDPOINT: {
          solution_j_imidpoint u_j(*sol_temp.box_jnh_aff, tn, newh,
                                   embedded_ode, atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case RADAU3: {
          solution_j_radau3 u_j(*sol_temp.box_jnh_aff, tn, newh, embedded_ode,
                                atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case HEUN: {
          solution_j_heun u_j(*sol_temp.box_jnh_aff, tn, newh, embedded_ode,
                              atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case TAYLOR4: {
          solution_j_tayl4 u_j(*sol_temp.box_jnh_aff, tn, newh, embedded_ode,
                               atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case LA3: {
          solution_j_la3 u_j(*sol_temp.box_jnh_aff, tn, newh, embedded_ode,
                             atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case LC3: {
          solution_j_lc3 u_j(*sol_temp.box_jnh_aff, tn, newh, embedded_ode,
                             atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case RK4: {
          solution_j_rk4 u_j(*sol_temp.box_jnh_aff, tn, newh, embedded_ode,
                             atol, fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff, embedded_ode,
                              monotony_active);
          sol_temp.flush();
          list_solution_j.push_back(u_j);
          break;
        }
        case RADAU3_DAE: {
          solution_j_dae u_j(*sol_temp_dae.box_jnh_aff,
                             *sol_temp_dae.box_xnh_aff, tn, newh, embedded_dae,
                             atol, fac);
          u_j.compute_oneStep(*sol_temp_dae.box_jnh_aff,
                              *sol_temp_dae.box_xnh_aff, embedded_dae);
          sol_temp_dae.flush();
          list_solution_j_dae.push_back(u_j);
          // cout << "\nSolution at t=" << sol_temp_dae.time_j.ub() << " : " <<
          // sol_temp_dae.box_jnh_aff->itv() << endl;
          break;
        }

        default: {
          /*solution_j u_j(*sol_temp.box_jnh_aff,tn,newh,
          embedded_ode,atol,fac);
          u_j.compute_oneStep(*sol_temp.box_jnh_aff,embedded_ode);
          sol_temp.flush();
          list_solution_j.push_back(u_j);*/
          std::cout << "This method does not exist !" << std::endl;
          tn = time_T;
        }
        }
      }
      if (!list_solution_j.empty())
        tn = sol_temp.time_j.ub();
      else
        tn = sol_temp_dae.time_j.ub();

    } while (tn < time_T);
    if (tn > 1e7) {
      tn = time_T;
    } else {
      if (!list_solution_j.empty()) {
        // print of the last solution
        solution_j sol_temp = list_solution_j.back();
        // sol_temp.print_soljn();
        std::cout << "\nSolution at t=" << tn << " : "
                  << sol_temp.box_jnh_aff->itv() << std::endl;
        // cout << "Diam : " << sol_temp.box_jnh->diam() << endl;

        LOGGER->Log("Solution at t=%f : ", tn);
        LOGGER->Log_sol(sol_temp.box_jnh_aff->itv());
        LOGGER->Log_end();

        sol_to_generic(list_solution_j);
        // list_solution_j.clear();

      } else {
        // print of the last solution
        solution_j_dae sol_temp_dae = list_solution_j_dae.back();
        std::cout << "\nSolution at t=" << tn << " : "
                  << sol_temp_dae.box_jnh_aff->itv() << std::endl;

        LOGGER->Log("Solution at t=%f : ", tn);
        LOGGER->Log_sol(sol_temp_dae.box_jnh_aff->itv());
        LOGGER->Log_end();

        sol_to_generic(list_solution_j_dae);
        // list_solution_j_dae.clear();
      }
    }

    return 1;
  };

  // find the box of a stack which contains a sub-box (stack size nbvar/2, y
  // size nbvar)
  IntervalVector find_box(std::list<IntervalVector> *stack,
                          const IntervalVector &y) {

    std::list<IntervalVector>::iterator iterator_list;

    IntervalVector suby = y.subvector(0, y.size() / 2 - 1);
    for (iterator_list = stack->begin(); iterator_list != stack->end();
         iterator_list++) {
      IntervalVector yv = *iterator_list;

      if (!(suby & yv).is_empty())
        return yv;
    }

    return IntervalVector(y.size() / 2);
  }

  int order_of_method(Method _meth) {
    switch (meth) {
    case IEULER: {
      return 2;
      break;
    }
    case IMIDPOINT: {
      return 3;
      break;
    }
    case RADAU3: {
      return 4;
      break;
    }
    case HEUN: {
      return 3;
      break;
    }
    case TAYLOR4: {
      return 5;
      break;
    }
    case LA3: {
      return 5;
      break;
    }
    case LC3: {
      return 5;
      break;
    }
    case RK4: {
      return 5;
      break;
    }
    case RADAU3_DAE: {
      return 4;
      break;
    }
    default: { return 5; }
    }
    return 5;
  }

  double pred_h(ivp_ode *_ode, double atol) {
    IntervalMatrix J =
        _ode->eval_jacobian_init(); // ode->ydot->jacobian(_ode->yinit);
    double max_J = 0;
    for (int i = 0; i < J.nb_rows(); i++) {
      for (int j = 0; j < J.nb_cols(); j++) {
        if (abs(J[i][j]).ub() > max_J)
          max_J = abs(J[i][j]).ub();
      }
    }
    return max_J;
  }

  // constructor
  simulation(ivp_ode *_ode, double T, Method _meth, double a) {
    process = 0;
    embedded_ode = _ode;
    time_T = T;
    atol = a;
    meth = _meth;
    nb_var = _ode->nbvar;
    monotony_active = false;
    double coef = pred_h(_ode, atol);
    if (coef == 0.0)
      coef = 1.0;
    double h = std::min(std::min(hmax, 0.01 / coef), T / 2.0);

    embedded_ode->frechet_precomputing(order_of_method(meth));

    solution_j u_j0(*embedded_ode->yinit_aff, embedded_ode->t0, h, atol);
    list_solution_j.push_back(u_j0);
  }

  // constructor
  simulation(ivp_ode *_ode, double T) {
    process = 0;
    embedded_ode = _ode;
    time_T = T;
    atol = 1e-6;
    meth = RK4;
    nb_var = _ode->nbvar;
    monotony_active = false;
    double coef = pred_h(_ode, atol);
    if (coef == 0.0)
      coef = 1.0;
    double h = std::min(std::min(hmax, 0.01 / coef), T / 2.0);

    embedded_ode->frechet_precomputing(order_of_method(meth));
    solution_j u_j0(*embedded_ode->yinit_aff, embedded_ode->t0, h, atol);
    list_solution_j.push_back(u_j0);
  }

  // constructor
  simulation(ivp_ode *_ode, double T, Method _meth) {
    process = 0;
    embedded_ode = _ode;
    time_T = T;
    atol = 1e-6;
    meth = _meth;
    nb_var = _ode->nbvar;
    monotony_active = false;
    double coef = pred_h(_ode, atol);
    if (coef == 0.0)
      coef = 1.0;
    double h = std::min(std::min(hmax, 0.01 / coef), T / 2.0);

    embedded_ode->frechet_precomputing(order_of_method(meth));
    solution_j u_j0(*embedded_ode->yinit_aff, embedded_ode->t0, h, atol);
    list_solution_j.push_back(u_j0);
  }

  // constructor
  simulation(ivp_ode *_ode, double T, double a) {
    process = 0;
    embedded_ode = _ode;
    time_T = T;
    atol = a;
    meth = RK4;
    nb_var = _ode->nbvar;
    monotony_active = false;
    double coef = pred_h(_ode, atol);
    if (coef == 0.0)
      coef = 1.0;
    double h = std::min(std::min(hmax, 0.01 / coef), T / 2.0);

    embedded_ode->frechet_precomputing(order_of_method(meth));
    solution_j u_j0(*embedded_ode->yinit_aff, embedded_ode->t0, h, atol);
    list_solution_j.push_back(u_j0);
  }

  /// for dae
  simulation(ivp_dae_h1 *_dae, double T, Method _meth, double a) {
    process = 0;
    embedded_dae = _dae;
    time_T = T;
    atol = a;
    meth = RADAU3_DAE;
    nb_var = _dae->nbvar;
    monotony_active = false;
    double h = 0.001;

    embedded_dae->frechet_precomputing(order_of_method(meth));

    solution_j_dae u_j0(*embedded_dae->yinit_aff, *embedded_dae->xinit_aff,
                        embedded_dae->t0, h, atol);
    u_j0.uniqueness(_dae);
    list_solution_j_dae.push_back(u_j0);
  }

  // constructor
  simulation(ivp_dae_h1 *_dae, double T) {
    process = 0;
    embedded_dae = _dae;
    time_T = T;
    atol = 1e-6;
    meth = RADAU3_DAE;
    nb_var = _dae->nbvar;
    monotony_active = false;
    double h = 0.001;
    embedded_dae->frechet_precomputing(order_of_method(meth));
    solution_j_dae u_j0(*embedded_dae->yinit_aff, *embedded_dae->xinit_aff,
                        embedded_dae->t0, h, atol);
    u_j0.uniqueness(_dae);
    list_solution_j_dae.push_back(u_j0);
  }

  // constructor
  simulation(ivp_dae_h1 *_dae, double T, Method _meth) {
    process = 0;
    embedded_dae = _dae;
    time_T = T;
    atol = 1e-6;
    meth = RADAU3_DAE;
    nb_var = _dae->nbvar;
    monotony_active = false;
    double h = 0.001;
    embedded_dae->frechet_precomputing(order_of_method(meth));
    solution_j_dae u_j0(*embedded_dae->yinit_aff, *embedded_dae->xinit_aff,
                        embedded_dae->t0, h, atol);
    u_j0.uniqueness(_dae);
    list_solution_j_dae.push_back(u_j0);
  }

  // constructor
  simulation(ivp_dae_h1 *_dae, double T, double a) {
    process = 0;
    embedded_dae = _dae;
    time_T = T;
    atol = a;
    meth = RADAU3_DAE;
    nb_var = _dae->nbvar;
    monotony_active = false;
    double h = 0.001;
    embedded_dae->frechet_precomputing(order_of_method(meth));
    solution_j_dae u_j0(*embedded_dae->yinit_aff, *embedded_dae->xinit_aff,
                        embedded_dae->t0, h, atol);
    u_j0.uniqueness(_dae);
    list_solution_j_dae.push_back(u_j0);
  }

  // destructor
  ~simulation() {
    /*list_solution_j.clear();
    list_solution_j_dae.clear();
    list_solution_g.clear(); */
    destructor();
  }

  void destructor() {
    for (auto &item : list_solution_j_dae) {
      item.destructor();
    }
    for (auto &item : list_solution_j) {
      item.destructor();
    }
    for (auto &item : list_solution_g) {
      item.destructor();
    }
  }

  // functions to test the solution
  IntervalVector get_last() {
    solution_g sol_temp = list_solution_g.back();
    return *sol_temp.box_jnh;
  }

  // final solution is include in a box
  bool finished_in(const IntervalVector &y_final) {
    if (y_final.size() != nb_var)
      return false;

    if (!list_solution_g.empty()) {
      solution_g sol_temp = list_solution_g.back();
      return sol_temp.box_jnh->is_subset(y_final);
    } else
      return false;
  }

  // final solution is include in at least one box of a list
  bool finished_in(std::list<IntervalVector> *stack) {
    std::list<IntervalVector>::iterator iterator_l;
    for (iterator_l = stack->begin(); iterator_l != stack->end();
         iterator_l++) {
      if (finished_in((IntervalVector)*iterator_l))
        return true;
    }
    return false;
  }

  // tube crosses a box
  bool has_crossed(const IntervalVector &y) {
    if (y.size() != nb_var)
      return false;

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        IntervalVector y_temp = *(iterator_list->box_j1);
        if (!((y_temp & y).is_empty()))
          return true;
      }
    }
    return false;
  }

  // tube stays in a box
  bool stayed_in(const IntervalVector &y_hull) {
    if (y_hull.size() != nb_var)
      return false;

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        IntervalVector y_temp = *(iterator_list->box_j1);
        if (!y_temp.is_subset(y_hull))
          return false;
      }
    }
    return true;
  }

  // at least one y(t) outside a box
  bool go_out(const IntervalVector &y_hull) {
    if (y_hull.size() != nb_var)
      return false;

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        IntervalVector y_temp = *(iterator_list->box_jnh);
        if ((y_temp & y_hull).is_empty())
          return true;
      }
    }
    return false;
  }

  // tube stays in a box till t
  bool stayed_in_till(const IntervalVector &y_hull, double t) {
    if (y_hull.size() != nb_var)
      return false;

    if (t < 0)
      return true;

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        IntervalVector y_temp = *(iterator_list->box_j1);
        if (iterator_list->time_j.lb() > t)
          return true;
        if (!y_temp.is_subset(y_hull))
          return false;
      }
    }
    return true;
  }

  // final solution crosses a box
  bool has_reached(const IntervalVector &y_final) {
    if (y_final.size() != nb_var)
      return false;

    if (!list_solution_g.empty()) {
      solution_g sol_temp = list_solution_g.back();
      return (!((*sol_temp.box_jnh) & y_final).is_empty());
    } else
      return false;
  }

  // final solution crosses at least one box of a list
  bool has_reached(std::list<IntervalVector> *stack) {
    std::list<IntervalVector>::iterator iterator_l;
    for (iterator_l = stack->begin(); iterator_l != stack->end();
         iterator_l++) {
      if (has_reached(*iterator_l))
        return true;
    }
    return false;
  }

  // one solution is inside at least one box of a list
  double one_in(std::list<IntervalVector> *stack) {
    std::list<IntervalVector>::iterator iterator_l;

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {

        IntervalVector y_temp = *iterator_list->box_jnh;
        double t = iterator_list->time_j.ub();

        for (iterator_l = stack->begin(); iterator_l != stack->end();
             iterator_l++) {
          if (y_temp.is_subset(*iterator_l))
            return t;
        }
      }
    }
    return (-1.0);
  }

  // return a box containing y(t)
  IntervalVector get(double t) {

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        if ((iterator_list->time_j).contains(t))
          return *iterator_list->box_j1;
      }
    }
    return IntervalVector(nb_var);
  }

  // return a tight box containing y(t)
  IntervalVector get_tight(double t) {

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        if ((iterator_list->time_j).contains(t)) {
          Affine3Vector *it_box_jn_aff =
              new Affine3Vector(*iterator_list->box_jn);
          double it_tn = iterator_list->time_j.lb();
          double it_h = t - it_tn;

          switch (meth) {
          case IEULER: {
            solution_j_ieuler u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode,
                                  atol, 1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          case IMIDPOINT: {
            solution_j_imidpoint u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode,
                                     atol, 1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          case RADAU3: {
            solution_j_radau3 u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode,
                                  atol, 1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          case HEUN: {
            solution_j_heun u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode, atol,
                                1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          case TAYLOR4: {
            solution_j_tayl4 u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode,
                                 atol, 1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          case LA3: {
            solution_j_la3 u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode, atol,
                               1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          case LC3: {
            solution_j_lc3 u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode, atol,
                               1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          case RK4: {
            solution_j_rk4 u_j(*it_box_jn_aff, it_tn, it_h, embedded_ode, atol,
                               1.0);
            u_j.compute_oneStep(*it_box_jn_aff, embedded_ode, monotony_active);
            if (u_j.time_j.ub() == t)
              return *u_j.box_jnh;
            else
              return *u_j.box_j1;
            break;
          }
          default:
            return get(t);
          }
        }
      }
    }
    return IntervalVector(nb_var);
  }

  // return a box contained in the next box (possibly an attractor)
  IntervalVector get_attractor() {

    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      iterator_list = list_solution_g.begin();
      IntervalVector attractor = *iterator_list->box_jnh;

      iterator_list++; // prob 2 val idem

      for (; iterator_list != list_solution_g.end(); iterator_list++) {
        if (attractor.is_subset(*iterator_list->box_jnh))
          return attractor;
        else
          attractor = *iterator_list->box_jnh;
      }
    }

    return IntervalVector(nb_var);
  }

  // return the domain covered by the simulation (the hull of y)
  IntervalVector get_domain() {
    if (!list_solution_g.empty()) {
      std::list<solution_g>::iterator iterator_list;
      iterator_list = list_solution_g.begin();

      IntervalVector hull = *iterator_list->box_jnh;

      for (; iterator_list != list_solution_g.end(); iterator_list++) {
        hull |= *iterator_list->box_jnh;
      }
      return hull;
    }

    return IntervalVector(nb_var);
  }

  // export in a file for ploting
  void export2d_yn(const char *filename, int x, int y) {
    assert(nb_var > 1);
    std::cout << "export in progress..." << std::endl;
    if (!list_solution_g.empty()) {
      std::ofstream file(filename, std::ios::out | std::ios::trunc);
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        file << iterator_list->box_jnh->operator[](x) << " ; "
             << iterator_list->box_jnh->operator[](y) << std::endl;
      }

      file.close();
    }
  };

  // export in a file for ploting
  void export3d_yn(const char *filename, int x, int y, int z) {
    assert(nb_var > 2);
    std::cout << "export in progress..." << std::endl;
    if (!list_solution_g.empty()) {
      std::ofstream file(filename, std::ios::out | std::ios::trunc);
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        file << iterator_list->box_jn->operator[](x) << " ; "
             << iterator_list->box_jn->operator[](y) << " ; "
             << iterator_list->box_jn->operator[](z) << std::endl;
      }

      file.close();
    }
  };

  // export in a file for ploting
  void export1d_yn(const char *filename, int x) {
    assert(nb_var > 1);
    std::cout << "export in progress..." << std::endl;
    if (!list_solution_g.empty()) {
      std::ofstream file(filename, std::ios::out | std::ios::trunc);
      std::list<solution_g>::iterator iterator_list;
      for (iterator_list = list_solution_g.begin();
           iterator_list != list_solution_g.end(); iterator_list++) {
        file << iterator_list->box_j1->operator[](x) << " ; "
             << iterator_list->time_j << std::endl;
      }

      file.close();
    }
  };
};
//}
} // namespace ibex

#endif
