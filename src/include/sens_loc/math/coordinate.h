#ifndef COORDINATE_H_Y7FJINQO
#define COORDINATE_H_Y7FJINQO

#include <gsl/gsl>
#include <sens_loc/math/eigen_types.h>
#include <type_traits>

namespace sens_loc::math {

/// This enum allows to discrimate in which coordinate system or frame of
/// reference a vector lives. It is solely for code readability and correctness
/// evaluation.
enum class frame {
    world,   ///< 3D, Global, World Coordinate System. \f$(U, V, W)\f$.
    camera,  ///< 3D, Camera coordinate system, but the camera center is the
             ///< frame reference. \f$(X, Y, Z)\f$
    sphere,  ///< 3D, projection from image into space. \f$(X_s, Y_s, Z_s)\f$.
             ///< Equivalent to the direction of a lightray for a pixel.
    image,   ///< 2D, coordinate in an image as floating point. \f$(x, y)\f$
    pixel,   ///< 2D, usually a discrete coordinate system that refers to pixels
             ///< in an image. \f$(u, v)\f$
};

namespace detail {
/// Helper type-traits to examine if a reference frame is 2D. If its is not
/// 2D, the coordinate system is 3D.
/// \sa math::frame
template <frame Frame>
struct is_2d
    : std::bool_constant<Frame == frame::image || Frame == frame::pixel> {};

/// Small helper class that holds the data for 2 dimensional coordinates.
/// \sa math::coordinate
template <typename Real>
struct coordinate2d {
    static_assert(std::is_arithmetic_v<Real>);

    coordinate2d()
        : d(Real(0.), Real(0.)) {}
    coordinate2d(Real x1, Real x2)
        : d(x1, x2) {}

    vec<Real, 2> d;
};

/// Small helper class that holds the data for 3 dimensional coordinates.
/// \sa math::coordinate
template <typename Real>
struct coordinate3d {
    static_assert(std::is_arithmetic_v<Real>);

    coordinate3d()
        : d(Real(0.), Real(0.), Real(0.)) {}
    coordinate3d(Real x1, Real x2, Real x3)
        : d(x1, x2, x3) {}

    vec<Real, 3> d;
};
}  // namespace detail

/// This class is a strongly typed coordinate class to ensure the implemented
/// math does not mix up reference frames.
///
/// The overall goal of this slightly complicated construct is to increase
/// readability of the code **USING** this class and to improve correctness.
///
/// This class allows only access with the coordinate conventions for each
/// frame.
/// It prevents mixing different coordinates while assignment or other
/// operations and therefore forces conversion functions. Such conversions
/// are well defined through camera intrinsics for example.
///
/// There are helper typedefs, for easier writing:
/// \c pixel_coord<Real>, \c image_coord<Real>, \c sphere_coord<Real>,
/// \c camera_coord<Real> and \c world_coord<Real>.
///
/// \code
///   const pixel_coord<int> pixel(42, 32);
///   pixel.u(); // Accessing the u-coordinate for that pixel.
///   pixel.x(); // DOES NOT COMPILE - WRONG CONVENTION!
///
///   // Changing coordinate systems requires an explicit transformation, for
///   // example with the camera calibration as in this case.
///   const camera_models::pinhole<float> i;
///   const image_coord<float> transformed = i.transform_to_image(pixel);
///   transformed.x() // Accessing x-coordinate for that pixel.
///                   // Same point but different coordinate system.
///   transformed.u() // DOES NOT COMPILE - WRONG CONVENTION!
///
///   // It is not possible to mix coordinates up.
///   pixel_coord<float> sub_pixel(42.42, 42.42);
///   image_coord<float> transformed = sub_pixel; // DOES NOT COMPILE
///                                               // TYPE MISSMATCH!!
/// \endcode
///
/// \tparam Real underlying type of the coordinates
/// \tparam Frame type of reference frame, results in either 2 or 3 dimensional
/// vectors. The vector data is inherited.
template <typename Real, frame Frame>
class coordinate
    : private std::conditional_t<detail::is_2d<Frame>::value,
                                 detail::coordinate2d<Real>,
                                 detail::coordinate3d<Real>> {
    static_assert(std::is_arithmetic_v<Real>,
                  "Arithmetic type for Real required!");

    using base_class = std::conditional_t<detail::is_2d<Frame>::value,
                                          detail::coordinate2d<Real>,
                                          detail::coordinate3d<Real>>;

  public:
    using real = Real;

    // Reuse constructors of baseclass
    using base_class::base_class;

    /// Calculate the euclidean norm for the coordinate.
    /// \note This function is only enabled if the \p Real type is a floating
    /// point number.
    template <typename T = Real,
              typename   = std::enable_if_t<!std::is_integral_v<T>>>
    [[nodiscard]] Real norm() const noexcept {
        return base_class::d.norm();
    }

    /// Return the normalized vector of this coordinate.
    /// \note This function is only enabled if the \p Real type is a floating
    /// point number and the vector is in 3 dimensional space.
    template <typename T = Real,
              typename U = frame,
              typename   = std::enable_if_t<!std::is_integral_v<T> &&
                                          !detail::is_2d<Frame>::value>>
    [[nodiscard]] coordinate<Real, Frame> normalized() const noexcept {
        const auto res = base_class::d.normalized();
        return {res[0], res[1], res[2]};
    }

    /// Calculate the dot product with \p other.
    [[nodiscard]] Real dot(const coordinate<Real, Frame>& other) const
        noexcept {
        return base_class::d.dot(other.d);
    }
    /// Calculate the cross product with \p other.
    template <
        typename T = frame,
        typename = std::enable_if_t<!(Frame == T::pixel || Frame == T::image)>>
    [[nodiscard]] coordinate<Real, Frame>
    cross(const coordinate<Real, Frame>& other) const noexcept {
        const auto res = base_class::d.cross(other.d);
        return {res[0], res[1], res[2]};
    }

    // clang-format off
    template <typename T = frame,
              typename   = std::enable_if_t<Frame == T::pixel>>
    [[nodiscard]] Real u() const noexcept { return base_class::d[0]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::pixel>>
    [[nodiscard]] Real v() const noexcept { return base_class::d[1]; }


    template <typename T = frame,
              typename   = typename std::enable_if_t<Frame == T::image>>
    [[nodiscard]] Real x() const noexcept { return base_class::d[0]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::image>>
    [[nodiscard]] Real y() const noexcept { return base_class::d[1]; }


    template <typename T = frame,
              typename   = typename std::enable_if_t<Frame == T::sphere>>
    [[nodiscard]] Real Xs() const noexcept { return base_class::d[0]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::sphere>>
    [[nodiscard]] Real Ys() const noexcept { return base_class::d[1]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::sphere>>
    [[nodiscard]] Real Zs() const noexcept { return base_class::d[2]; }


    template <typename T = frame,
              typename   = typename std::enable_if_t<Frame == T::camera>>
    [[nodiscard]] Real X() const noexcept { return base_class::d[0]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::camera>>
    [[nodiscard]] Real Y() const noexcept { return base_class::d[1]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::camera>>
    [[nodiscard]] Real Z() const noexcept { return base_class::d[2]; }


    template <typename T = frame,
              typename   = typename std::enable_if_t<Frame == T::world>>
    [[nodiscard]] Real U() const noexcept { return base_class::d[0]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::world>>
    [[nodiscard]] Real V() const noexcept { return base_class::d[1]; }
    template <typename T = frame,
              typename   = typename std::enable_if<Frame == T::world>>
    [[nodiscard]] Real W() const noexcept { return base_class::d[2]; }
    // clang-format on

    coordinate<Real, Frame> operator-(const coordinate<Real, Frame>& p) const
        noexcept {
        coordinate<Real, Frame> res;
        auto                    difference = base_class::d - p.d;

        res.d = difference;
        return res;
    }
    coordinate<Real, Frame> operator+(const coordinate<Real, Frame>& p) const
        noexcept {
        coordinate<Real, Frame> res;
        auto                    sum = base_class::d + p.d;

        res.d = sum;
        return res;
    }
    coordinate<Real, Frame> operator*(const float& f) const
        noexcept {
        coordinate<Real, Frame> res;
        res.d[0] = base_class::d[0] * f;
        res.d[1] = base_class::d[1] * f;
        res.d[2] = base_class::d[2] * f;

        return res;
    }
};

template <typename Real>
coordinate<Real, frame::camera>
operator*(Real factor, const coordinate<Real, frame::camera>& pt) noexcept {
    static_assert(std::is_floating_point_v<Real>,
                  "Real must be a floating point type for scaling operations!");

    coordinate<Real, frame::camera> result{factor * pt.X(), factor * pt.Y(),
                                           factor * pt.Z()};
    Ensures(std::abs(factor * pt.norm() - result.norm()) < Real(0.0001));
    return result;
}

/// Scaling spherical coordinates results in camera coordinates.
template <typename Real>
coordinate<Real, frame::camera>
operator*(Real factor, const coordinate<Real, frame::sphere>& pt) noexcept {
    static_assert(std::is_floating_point_v<Real>,
                  "Real must be a floating point type for scaling operations!");
    coordinate<Real, frame::camera> result{factor * pt.Xs(), factor * pt.Ys(),
                                           factor * pt.Zs()};
    Ensures(std::abs(factor * pt.norm() - result.norm()) < Real(0.0001));
    return result;
}

template <typename Real = int>
using pixel_coord = coordinate<Real, frame::pixel>;
template <typename Real = float>
using image_coord = coordinate<Real, frame::image>;
template <typename Real = float>
using sphere_coord = coordinate<Real, frame::sphere>;
template <typename Real = float>
using camera_coord = coordinate<Real, frame::camera>;
template <typename Real = float>
using world_coord = coordinate<Real, frame::world>;

}  // namespace sens_loc::math

#endif /* end of include guard: COORDINATE_H_Y7FJINQO */
