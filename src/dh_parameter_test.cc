#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DhTest

#include "dh_parameter.h"
#include <cmath>
#include <vector>

#include <boost/test/unit_test.hpp>

void CheckCloseTables(const std::vector<std::vector<double>>& vec_table_rot,
                      const std::vector<double>& vec_table_trans,
                      const DhParameter& dh_table, double eps = 0.0001) {
  for(int i(0); i < 3; ++i)
    for(int j(0); j < 3; ++j) {
      double expected(vec_table_rot[i][j]), 
             returned(dh_table.rotation()(i, j));
      if(std::fabs(expected) < eps) 
        BOOST_CHECK_SMALL(returned, eps);
      else
        BOOST_CHECK_CLOSE(expected, returned, eps);
    }
  
  for(int i(0); i < 3; ++i) {
      double expected(vec_table_trans[i]), 
             returned(static_cast<double>(dh_table.translation()[i]));
      if(std::fabs(expected) < eps) 
        BOOST_CHECK_SMALL(returned, eps);
      else
        BOOST_CHECK_CLOSE(expected, returned, eps);
  }
}

BOOST_AUTO_TEST_CASE(constructor) {
  const double pi_6 = M_PI / 6;
  CheckCloseTables({{0.866, 0.0, 0.5},
                   {   0.5, 0.0, -0.866},                 
                   {   0.0, 1.0, 0.0}}, 
                   {0.217, 0.125, 0}, 
                   DhParameter(pi_6, 0, 0.25, M_PI_2), 1);
}

