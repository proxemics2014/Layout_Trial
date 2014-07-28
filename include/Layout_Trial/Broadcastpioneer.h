#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

#ifndef BROADCASTPIONEER_H_
#define BROADCASTPIONEER_H_
class TransformBroadcasterExample
{
protected:
    ros::Rate _loop_rate;
    std::string _primary_target;
    std::string _fallback_target;
    static tf::TransformBroadcaster _tf_broadcaster;

public:
    TransformBroadcasterExample( double const & loop_rate_hz = 10 )
    :
        _loop_rate( loop_rate_hz ),
        // default to looking at /world frame
        _primary_target( "/world" ),
        _fallback_target( "/world" )
    {
        //
    }

    // do work until CTRL+C
    void spin()
    {
        while( ros::ok() )
        {
            spinOnce();
            ros::spinOnce();
            _loop_rate.sleep();
        }
    }
    // send transform to TF server
    void publishTransform( tf::Transform const & transform, std::string const & from_frame, std::string const & to_frame )
    {
        _tf_broadcaster.sendTransform( tf::StampedTransform( transform, ros::Time::now(), from_frame, to_frame ) );
    }

    // do one iteration of work
    void spinOnce()
    {
        // Quaternion( Y, P, R ) OR Quaternion( X, Y, Z, W )
        // Vector3( X, Y, Z )
        static tf::Transform const zero_transform( tf::Quaternion( 0, 0, 0 ), tf::Vector3( 0, 0, 0 ) );

        if( !_primary_target.empty() ) publishTransform( zero_transform, _primary_target ,"/bandit/primary_target" );
        if( !_fallback_target.empty() ) publishTransform( zero_transform, _fallback_target,"/bandit/fallback_target" );
    }

    // set primary target name
    void selectPrimaryTarget( std::string const & frame_name )
    {
        _primary_target = frame_name;
    }

    // set fallback target name
    void selectFallbackTarget( std::string const & frame_name )
    {
        _fallback_target = frame_name;
    }
};

#endif