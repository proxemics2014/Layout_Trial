#include <ros/ros.h>

#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>

#include <bandit_msgs/JointArray.h>
#include <bandit/joint_name.h>

class BanditLookAtTargetNode
{
public:
    typedef bandit_msgs::Joint _BanditJointMsg;
    typedef bandit_msgs::JointArray _BanditJointArrayMsg;

private:
    ros::NodeHandle _nh_rel;
    ros::Rate _loop_rate;

protected:
    tf::TransformListener _tf_listener;
    tf::TransformBroadcaster _tf_broadcaster;
    ros::Publisher _joint_array_publisher;
    std::string const _primary_target_frame_name;
    std::string const _fallback_target_frame_name;
    tf::Transform _bandit_neck_to_bandit_eyes_tf;

public:
    BanditLookAtTargetNode
    (
        ros::NodeHandle & nh_rel,
        std::string const & primary_target_frame_name = "/bandit/primary_target",
        std::string const & fallback_target_frame_name = "/bandit/fallback_target",
        double const & loop_rate_hz = 30
    )
    :
        _nh_rel( nh_rel ),
        _loop_rate( loop_rate_hz ),
        _tf_listener( _nh_rel ),
        _tf_broadcaster(),
        _joint_array_publisher( _nh_rel.advertise<_BanditJointArrayMsg>( "joint_array_command", 1 ) ),
        _primary_target_frame_name( primary_target_frame_name ),
        _fallback_target_frame_name( fallback_target_frame_name )
    {
        //
    }

    void spin()
    {
        spinFirst();

        while( ros::ok() )
        {
            spinOnce();
            ros::spinOnce();
            _loop_rate.sleep();
        }
    }

    void spinFirst()
    {
        bool transforms_set = false;

        while( ros::ok() && !transforms_set )
        {
            if( transformExists( "/bandit/head_tilt_link", "/bandit/pupil_left_link" ) && transformExists( "/bandit/pupil_left_link", "/bandit/pupil_right_link" ) )
            {
                tf::StampedTransform const head_tilt_to_l_eye_tf = lookupTransform( "/bandit/head_tilt_link", "/bandit/pupil_left_link" );
                tf::StampedTransform const l_eye_to_r_eye_tf = lookupTransform( "/bandit/pupil_left_link", "/bandit/pupil_right_link" );
                tf::Transform const l_eye_to_eyes_tf( tf::Quaternion( 0, 0, 0, 1 ), l_eye_to_r_eye_tf.getOrigin() / 2 );

                _bandit_neck_to_bandit_eyes_tf = head_tilt_to_l_eye_tf * l_eye_to_eyes_tf;

                transforms_set = true;
            }
        }
    }

    void spinOnce()
    {
        lookAtTarget( _primary_target_frame_name );
    }

    bool transformExists( std::string const & from_frame, std::string const & to_frame )
    {
        return _tf_listener.canTransform( to_frame, from_frame, ros::Time( 0 ) );
    }

    tf::StampedTransform lookupTransform( std::string const & from_frame, std::string const & to_frame )
    {
        tf::StampedTransform stamped_transform;

        try
        {
            _tf_listener.lookupTransform( to_frame, from_frame, ros::Time( 0 ), stamped_transform );
        }
        catch( tf::TransformException & e )
        {
            ROS_WARN( "Lookup failed: %s", e.what() );
        }

        return stamped_transform;
    }

    // roll, pitch, yaw
    tf::Vector3 getRotationBetween( std::string const & from_frame, std::string const & to_frame )
    {
        double r, p, y;

        tf::StampedTransform const stamped_transform = lookupTransform( from_frame, to_frame );

        stamped_transform.getBasis().getEulerYPR( y, p, r );

        return tf::Vector3( r, p, y );
    }

    void lookAtTarget( std::string const & target_name, bool const & allow_fallback = true )
    {
        tf::StampedTransform bandit_neck_to_target_tf;

        bool transform_invalid;

        if( transformExists( "/bandit/neck", target_name ) )
        {
            // get the transform to the target
            bandit_neck_to_target_tf = lookupTransform( "/bandit/neck", target_name );
            double const target_age = ( ros::Time::now() - bandit_neck_to_target_tf.stamp_ ).toSec();

            if( target_age > 0.5 )
            {
                ROS_WARN( "Transform from /bandit/neck to %s is older than 0.5 seconds (%f)", target_name.c_str(), target_age );
                transform_invalid = true;
            }
        }
        else
        {
            ROS_WARN( "Transform from /bandit/neck to %s doesn't exist!", target_name.c_str() );
            transform_invalid = true;
        }

        if( transform_invalid )
        {
            // if allowed, try our fallback target
            if( allow_fallback )
            {
                ROS_WARN( "Looking at fallback frame %s", _fallback_target_frame_name.c_str() );
                lookAtTarget( _fallback_target_frame_name, false );
            }
            // otherwise bail out
            else
            {
                ROS_ERROR( "Unable to look up fallback frame; idling..." );
            }
            return;
        }

        // calculate neck pitch / neck yaw angle
        double const a = -asin( _bandit_neck_to_bandit_eyes_tf.getOrigin().z() / bandit_neck_to_target_tf.getOrigin().x() );
        double const b = atan2( bandit_neck_to_target_tf.getOrigin().z(), bandit_neck_to_target_tf.getOrigin().x() );

        double const pitch = a + b;
        double const yaw = atan2( bandit_neck_to_target_tf.getOrigin().x(), -bandit_neck_to_target_tf.getOrigin().y() ) - M_PI_2;

        // publish joint angles
        _BanditJointArrayMsg joint_array_msg;
        joint_array_msg.joints.resize( 2 );
        joint_array_msg.joints[0].id = bandit::JointName( "head_tilt_joint" );
        joint_array_msg.joints[0].angle = pitch;
        joint_array_msg.joints[1].id = bandit::JointName( "head_pan_joint" );
        joint_array_msg.joints[1].angle = yaw;

        _joint_array_publisher.publish( joint_array_msg );
    }
};

int main( int argc, char ** argv )
{
    ros::init( argc, argv, "bandit_look_at_node" );
    ros::NodeHandle nh_rel( "~" );

    BanditLookAtTargetNode look_at_target_node( nh_rel );
    look_at_target_node.spin();

    return 0;
}
