#ifndef POINTCLOUD_H_8O5EBVHZ
#define POINTCLOUD_H_8O5EBVHZ

#include <Eigen/Core>
#include <algorithm>
#include <gsl/gsl>
#include <sens_loc/math/coordinate.h>
#include <sens_loc/math/image.h>

namespace sens_loc::math {

/// A pointcloud is a set of camera coordinates.
template <typename Real>
using pointcloud   = std::vector<camera_coord<Real>>;
using pointcloud_t = pointcloud<float>;

/// A set of pixel coordinates.
template <typename Real>
using imagepoints   = std::vector<pixel_coord<Real>>;
using imagepoints_t = imagepoints<float>;

/// Calculate the point-wise euclidean distance between each point in \c c0
/// and \c c1.
/// The result is a row-vector with the distance for point 'i' at position 'i'.
/// \tparam one of \c pointcloud or \c imagepoints.
/// \pre c0.size() == c1.size() => the same number of points
/// \post each element in the result is non-negative.
template <typename Points>
std::vector<typename Points::value_type::real>
pointwise_distance(const Points& c0, const Points& c1) noexcept {
    Expects(c0.size() == c1.size());

    std::vector<typename Points::value_type::real> distances;
    if (c0.empty())
        return distances;

    distances.reserve(c0.size());
    for (std::size_t i = 0; i < c0.size(); ++i)
        distances.emplace_back((c0[i] - c1[i]).norm());

    Ensures(distances.size() == c0.size());
    Ensures(distances.size() == c1.size());
    Ensures(*std::min_element(distances.begin(), distances.end()) >= 0.0F);

    return distances;
}

/// A pose is an affine transformation in 3 dimensions. Internally this is
/// processed with homogeneous coordinates.
/// \sa pointcloud_t
using pose_t = Eigen::Matrix4f;

/// Calculate the relative pose to get from \c from to \c to.
///
/// Let \c O be the origin of a coordinate system, \c from and \c to are both
/// absolute poses in this coordinate system.
/// \c relative_pose calculates the relative pose to get from \c from to \c to.
/// \pre both poses need to be valid
/// \pre \c from must be invertable => pose is valid
inline pose_t relative_pose(const pose_t& from, const pose_t& to) noexcept {
    return from.inverse() * to;
}

/// Translate and rotate all points in \c points with the transformation \c p.
inline pointcloud_t operator*(const pose_t&       p,
                              const pointcloud_t& points) noexcept {
    pointcloud_t result;
    result.reserve(points.size());

    for (const auto& pt : points) {
        Eigen::Vector4f tr = p * Eigen::Vector4f{pt.X(), pt.Y(), pt.Z(), 1.0F};
        result.emplace_back(tr.x(), tr.y(), tr.z());
    }
    return result;
}

}  // namespace sens_loc::math

#endif /* end of include guard: POINTCLOUD_H_8O5EBVHZ */
