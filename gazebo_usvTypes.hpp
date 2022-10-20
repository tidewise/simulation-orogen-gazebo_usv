#ifndef gazebo_usv_TYPES_HPP
#define gazebo_usv_TYPES_HPP

/* If you need to define types specific to your oroGen components, define them
 * here. Required headers must be included explicitly
 *
 * However, it is common that you will only import types from your library, in
 * which case you do not need this file
 */
#include <base/Eigen.hpp>

namespace gazebo_usv {
    struct WaveAttributes
    {
        base::Vector3d wave_amplitude;
        base::Vector3d wave_frequency;
        double roll_amplitude;
        double roll_frequency;
    };
}

#endif

