#ifndef DH_PARAMETER_H_INCLUDED
#define DH_PARAMETER_H_INCLUDED

#include <flann/flann.hpp>
#include <vector>
#include <string>
#include <memory>
#include "PQP/PQP.h"
#include "Eigen/Dense"


typedef Eigen::Matrix<double, 3, 3, Eigen::RowMajor> EMatrix;
typedef Eigen::Vector3d EVector;
typedef PQP_REAL PqpQueryType[3];  //TODO: get proper return type

class DhParameter {
public:
  DhParameter(const double theta, const double d, 
              const double a, const double alpha);
  
  const EMatrix& rotation () const { return rotation_; }
  const EVector& translation () const { return translation_; }
  EMatrix& rotation () { return rotation_; }
  EVector& translation () { return translation_; }
  const PqpQueryType* RotData();
  const PQP_REAL* TransData ();
private:
  EMatrix rotation_;
  EVector translation_;
};


#endif // DH_PARAMETER_H_INCLUDED
