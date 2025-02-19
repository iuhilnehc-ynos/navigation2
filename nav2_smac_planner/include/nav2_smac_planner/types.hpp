// Copyright (c) 2020, Samsung Research America
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License. Reserved.

#ifndef NAV2_SMAC_PLANNER__TYPES_HPP_
#define NAV2_SMAC_PLANNER__TYPES_HPP_

#include <vector>
#include <utility>
#include <string>
#include <memory>

#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "nav2_util/node_utils.hpp"

namespace nav2_smac_planner
{

typedef std::pair<float, unsigned int> NodeHeuristicPair;

/**
 * @struct nav2_smac_planner::SearchInfo
 * @brief Search properties and penalties
 */
struct SearchInfo
{
  float minimum_turning_radius;
  float non_straight_penalty;
  float change_penalty;
  float reverse_penalty;
  float cost_penalty;
  float analytic_expansion_ratio;
  float analytic_expansion_max_length;
  std::string lattice_filepath;
  bool cache_obstacle_heuristic;
  bool allow_reverse_expansion;
};

/**
 * @struct nav2_smac_planner::SmootherParams
 * @brief Parameters for the smoother
 */
struct SmootherParams
{
  /**
   * @brief A constructor for nav2_smac_planner::SmootherParams
   */
  SmootherParams()
  : holonomic_(false)
  {
  }

  /**
   * @brief Get params from ROS parameter
   * @param node Ptr to node
   * @param name Name of plugin
   */
  void get(std::shared_ptr<rclcpp_lifecycle::LifecycleNode> node, const std::string & name)
  {
    std::string local_name = name + std::string(".smoother.");

    // Smoother params
    nav2_util::declare_parameter_if_not_declared(
      node, local_name + "tolerance", rclcpp::ParameterValue(1e-10));
    node->get_parameter(local_name + "tolerance", tolerance_);
    nav2_util::declare_parameter_if_not_declared(
      node, local_name + "max_iterations", rclcpp::ParameterValue(1000));
    node->get_parameter(local_name + "max_iterations", max_its_);
    nav2_util::declare_parameter_if_not_declared(
      node, local_name + "w_data", rclcpp::ParameterValue(0.2));
    node->get_parameter(local_name + "w_data", w_data_);
    nav2_util::declare_parameter_if_not_declared(
      node, local_name + "w_smooth", rclcpp::ParameterValue(0.3));
    node->get_parameter(local_name + "w_smooth", w_smooth_);
    nav2_util::declare_parameter_if_not_declared(
      node, local_name + "do_refinement", rclcpp::ParameterValue(true));
    node->get_parameter(local_name + "do_refinement", do_refinement_);
  }

  double tolerance_;
  int max_its_;
  double w_data_;
  double w_smooth_;
  bool holonomic_;
  bool do_refinement_;
};

/**
 * @struct nav2_smac_planner::MotionPose
 * @brief A struct for poses in motion primitives
 */
struct MotionPose
{
  /**
   * @brief A constructor for nav2_smac_planner::MotionPose
   */
  MotionPose() {}

  /**
   * @brief A constructor for nav2_smac_planner::MotionPose
   * @param x X pose
   * @param y Y pose
   * @param theta Angle of pose
   */
  MotionPose(const float & x, const float & y, const float & theta)
  : _x(x), _y(y), _theta(theta)
  {}

  MotionPose operator-(const MotionPose & p2)
  {
    return MotionPose(this->_x - p2._x, this->_y - p2._y, this->_theta - p2._theta);
  }

  float _x;
  float _y;
  float _theta;
};

typedef std::vector<MotionPose> MotionPoses;

/**
 * @struct nav2_smac_planner::LatticeMetadata
 * @brief A struct of all lattice metadata
 */
struct LatticeMetadata
{
  float min_turning_radius;
  float grid_resolution;
  unsigned int number_of_headings;
  std::vector<float> heading_angles;
  unsigned int number_of_trajectories;
  std::string motion_model;
};

/**
 * @struct nav2_smac_planner::MotionPrimitive
 * @brief A struct of all motion primitive data
 */
struct MotionPrimitive
{
  unsigned int trajectory_id;
  float start_angle;
  float end_angle;
  float turning_radius;
  float trajectory_length;
  float arc_length;
  float straight_length;
  bool left_turn;
  MotionPoses poses;
};

typedef std::vector<MotionPrimitive> MotionPrimitives;
typedef std::vector<MotionPrimitive *> MotionPrimitivePtrs;

}  // namespace nav2_smac_planner

#endif  // NAV2_SMAC_PLANNER__TYPES_HPP_
