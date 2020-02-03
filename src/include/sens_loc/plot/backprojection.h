#ifndef BACKPROJECTION_H_Y2GMMJKT
#define BACKPROJECTION_H_Y2GMMJKT

#include <opencv2/core/mat.hpp>
#include <sens_loc/math/image.h>
#include <sens_loc/math/pointcloud.h>

namespace sens_loc {

/// This namespace contains useful functions for plotting results,
/// matches and other utility for visualization.
namespace plot {

/// Plot matching keypoints onto the original images to display the accuracy
/// of a backprojection from one frame to another frame.
///
/// Keypoints that are obiously invalid because one of their coordinates is
/// negative are skipped.
/// \note The projection functions give points that are not visible anymore
/// the pixel_coord{-1, -1}
/// \sa camera_to_pixel
/// \param feature_file image the keypoints were detected on
/// \param keypoints_this keypoints detected on \c feature_file and matched
/// with keypoints of another frame.
/// \param keypoints_other matched keypoints to \c keypoints_this
/// \pre keypoints_this.size() == keypoints_other.size()
/// \pre the keypoints must have been matched before
/// \pre keypoints_this[i] corresponds to keypoints_other[i] for all i
/// \pre keypoints_other is projected on the image plane were \c keypoints_this
/// and \c feature_file live.
/// \returns image where the keypoints are plotted in different colors and
/// a connecting line between each match is drawn.
cv::Mat backprojection_correspondence(
    const math::image<uchar>&  feature_file,
    const math::imagepoints_t& keypoints_this,
    const math::imagepoints_t& keypoints_other) noexcept;

}  // namespace plot
}  // namespace sens_loc

#endif /* end of include guard: BACKPROJECTION_H_Y2GMMJKT */
