#include <flann/flann.hpp>
#include <chrono>
#include <random>
#include <vector>
#include <cstdio>
#include <array>

const unsigned kSetSize = 100000;
const unsigned kQuerySize = 10000;
const unsigned kDimSize = 12;
const unsigned kNN = 5;

void GenerateRandomVector(std::vector<std::vector<double>>& v, size_t set_size,
                          size_t dim_size) {
  std::random_device rand_device;
  std::mt19937 rand_generator;
  rand_generator.seed(rand_device());
  std::uniform_real_distribution<> real_dist(0, 1);

  v = std::vector<std::vector<double>> (set_size,
                                        std::vector<double> (dim_size));
  for(auto& row : v)
    for(auto& cell : row)
      cell = real_dist(rand_generator);
}

long MeasureTimeFlann(std::vector<std::vector<double>>& dataset_vector,
                      std::vector<std::vector<double>>& query_vector) {
  auto start = std::chrono::steady_clock::now();
  flann::Matrix<int> indices(new int[kQuerySize * kNN], kQuerySize, kNN);
  flann::Matrix<double> dists(new double[kQuerySize * kNN], kQuerySize, kNN);

  flann::Index<flann::L2<double>> index(
                                 flann::Matrix<double> (
                                   std::vector<double> (dataset_vector[0]).data(),
                                   1, kDimSize),
                                 flann::KDTreeIndexParams(4));

  for(size_t i(1); i < dataset_vector.size(); i++) {
    index.addPoints(flann::Matrix<double> (
                      std::vector<double> (dataset_vector[i]).data(),
                      1, kDimSize));
  }
  index.buildIndex();

  std::array<double, kQuerySize * kDimSize> query_array;

  for(unsigned k(0); k < kDimSize; k++) {
    for(unsigned i(0); i < kQuerySize; i++)
      query_array[i + k * kDimSize]  = query_vector.at(i).at(k);
  }

  flann::Matrix<double> query(query_array.data(), kQuerySize, kDimSize);
  index.knnSearch(query, indices, dists, kNN, flann::SearchParams(128));
  auto goal = std::chrono::steady_clock::now();

  delete[] indices.ptr();
  delete[] dists.ptr();
  //delete[] query.ptr();

  auto diff = goal - start;
  return diff.count() /1000000;
}

//int MeasureTimeRtree(std::vector<std::vector<double>>& dataset_vector,
//                     std::vector<std::vector<double>>& query_vector)
//{
//  auto start = std::chrono::steady_clock::now();
//  namespace bg = boost::geometry;
//  namespace bgi = boost::geometry::index;
//
//  typedef boost::array<double, kDimSize> point;
//  typedef bg::model::box<point> box;
//
//  bgi::rtree<point, bgi::quadratic<16>> rtree;
//  for(auto& i : dataset_vector)
//  {
//    rtree.insert(point{i});
//  }
//
//  std::vector<std::vector<point>> results(kQuerySize, std::vector<point>(
//      kDimSize));
//  for(unsigned i(0); i < kQuerySize; i++)
//    {
//      rtree.query(bgi::nearest(point(query_vector.at(i), kNN),
//                               std::back_inserter(results.at(i)));
//    }
//
//  auto goal = std::chrono::steady_clock::now();
//  auto diff = goal - start;
//  return diff.count();
//}

int main()
{
  std::vector<std::vector<double>> dataset_vector;
  GenerateRandomVector (dataset_vector, kSetSize, kDimSize);
  std::vector<std::vector<double>> query_vector;
  GenerateRandomVector (query_vector, kQuerySize, kDimSize);

  printf ("%d", MeasureTimeFlann (dataset_vector, query_vector));
  return 0;
}

