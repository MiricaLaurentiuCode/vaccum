/**
 * @file vacuum_bot.cpp
 * @brief This file contains function definitions for vacuum_bot class. 
 * 
 *
 * @author Vaibhav Bhilare
 * @copyright 2017, Vaibhav Bhilare
 *
 * MIT License
 * Copyright (c) 2017 Vaibhav Bhilare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* --Includes-- */
#include <vector>
#include "../include/vacuum_bot.h"

vacuum_bot::vacuum_bot(ros::NodeHandle& n,
            std::vector<std::vector<double>>& Goals) {
  //  Tell the action client that we want to spin a thread by default
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
                                          ac("move_base", true);

  //  Wait for the action server to come up
  while (!ac.waitForServer(ros::Duration(35.0))) {
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  ROS_INFO("Connected to move_base action server");
  move_base_msgs::MoveBaseGoal goal;
  for (auto Next_Goal : Goals) {
  vacuum_bot::Set_Current_X(Next_Goal, goal);
  vacuum_bot::Set_Current_Y(Next_Goal, goal);
  vacuum_bot::Set_Current_Orientation(Next_Goal, goal);
  //  We'll send a goal to the robot to move to new goal
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  ROS_INFO("Sending Robot to (%.2f, %.2f, %.2f)", Next_Goal.at(0),
                                Next_Goal.at(1), Next_Goal.at(2));
  ac.sendGoal(goal);
  ac.waitForResult(ros::Duration(30.0));
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Robot Moved to (%.2f, %.2f, %.2f)", Next_Goal.at(0),
                                Next_Goal.at(1), Next_Goal.at(2));
  else
    ROS_INFO("Robot failed to Reach the Goal");
}
}
void vacuum_bot::Set_Current_X(std::vector<double>& Next_Goal,
                          move_base_msgs::MoveBaseGoal & goal) {
goal.target_pose.pose.position.x = Next_Goal.at(0);
}

void vacuum_bot::Set_Current_Y(std::vector<double>& Next_Goal,
                          move_base_msgs::MoveBaseGoal & goal) {
goal.target_pose.pose.position.y = Next_Goal.at(1);
}

void vacuum_bot::Set_Current_Orientation(std::vector<double>& Next_Goal,
                                   move_base_msgs::MoveBaseGoal & goal) {
auto Angle_Degrees = Next_Goal.at(2);
auto Radians = Angle_Degrees*(3.14159/180);
auto quaternion = tf::createQuaternionFromYaw(Radians);
geometry_msgs::Quaternion qMsg;
tf::quaternionTFToMsg(quaternion, qMsg);
goal.target_pose.pose.orientation = qMsg;
}
