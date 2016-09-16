#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>

#include <string.h>

//==== Global variables ====//
ros::Publisher pose_pub;
geometry_msgs::PoseStamped pose_out;

void tf_callback( const geometry_msgs::TransformStamped::ConstPtr& msg ) {
	//==== Header ====//
	pose_out.header.seq = msg->header.seq;
	pose_out.header.stamp = ros::Time::now();
	pose_out.header.frame_id = msg->header.frame_id;

	//==== Position ====//
	pose_out.pose.position.x = msg->transform.translation.x;
	pose_out.pose.position.y = msg->transform.translation.y;
	pose_out.pose.position.z = msg->transform.translation.z;

	pose_out.pose.orientation.w = msg->transform.rotation.w;
	pose_out.pose.orientation.x = msg->transform.rotation.x;
	pose_out.pose.orientation.y = msg->transform.rotation.y;
	pose_out.pose.orientation.z = msg->transform.rotation.z;

	pose_pub.publish( pose_out );
}

int main( int argc, char **argv ) {
	//==== Initialize node ====//
	ros::init( argc, argv, "transform_rebroadcaster" );
	ros::NodeHandle nh( ros::this_node::getName() );

	//==== Set up variables ====//
	std::string input_topic = "transform";
	std::string output_topic = "pose";

	//==== Get parameters if set ====//
	if( !nh.getParam( "input_transform", input_topic ) ) {
		ROS_WARN( "No parameter set for \"input_transform\", using: %s", input_topic.c_str() );
	} else {
		ROS_INFO( "Listening for transform on: %s", input_topic.c_str() );
	}

	if( !nh.getParam( "output_pose", output_topic ) ) {
		ROS_WARN( "No parameter set for \"output_pose\", using: %s", output_topic.c_str() );
	} else {
		ROS_INFO( "Rebroadcasting pose on: %s", output_topic.c_str() );
	}

	//==== Begin publisher & subscriber ====//
	pose_pub = nh.advertise<geometry_msgs::PoseStamped>(output_topic, 1000);
	ros::Subscriber tf_sub = nh.subscribe( input_topic, 1000, tf_callback );

	//==== Begin listening for topics ====//
	ros::spin();

	return 0;
}

