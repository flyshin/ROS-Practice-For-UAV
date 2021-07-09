/*

This code is waypoint of forming diamond. It is first time for me to do coding of C++.
So. In main fuction, there is the algorithm of waypoint of forming diamond.

Code detail

In code, mode is each waypoint mode about 4 point.  


Future work is to coding moving the algorithm of waypoint from  main fuction to under main function-> in definetion. 


*/







#include <ros/ros.h>
#include <std_msgs/String.h> 
#include <stdio.h>
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Vector3Stamped.h"



  // Wapoint flight mode 




// mesage 
geometry_msgs::PoseStamped current_state;
void state_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{

 current_state =  *msg;
}


// mode chang


int mode_change(int mode1)
{
   int mode = mode1+1;
    
   return mode;
}
  



int mode_change_recusive(int mode1)
{
	int mode =mode1-3;

	return mode;
}






int main(int argc, char **argv)
{
   ros::init(argc, argv, "pub_setpoints");
   ros::NodeHandle n;

   ros::Publisher dm_pub = n.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",100);
   ros::Subscriber state_sub = n.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose",100,state_cb);
  
   ros::Rate loop_rate(100);
   ros::spinOnce();

   geometry_msgs::PoseStamped pos;
   int count = 1;
   int mode = 0; 

   
	int debugCount = 0;
		//PositionReciever qp;:
 		//Body some_object;
		//qp.connect_to_server();

	
   while(ros::ok())
   {
	   //some_object = qp.getStatus();
		// some_object.print();
		//printf("%f\n",some_object.position_x);
       pos.header.stamp = ros::Time::now();
       pos.header.seq=count;
       pos.header.frame_id = 1;

       
       //pos.pose.position.x =  0;//0.001*some_object.position_x;
       //pos.pose.position.y = 10.0;//0.001*some_object.position_y;
      // pos.pose.position.z = 10.0;//0.001*some_object.position_z;       
      
       pos.pose.orientation.x = 0;
       pos.pose.orientation.y = 0;
       pos.pose.orientation.z = 0;
       pos.pose.orientation.w = 1;
      

  


     if( mode == 0 )
     { 
      
       
       pos.pose.position.x =  0;//0.001*some_object.position_x;
       pos.pose.position.y =  10.0;//0.001*some_object.position_y;
       pos.pose.position.z = 10.0;//0.001*some_object.position_z;       
      
     
     
      if (current_state.pose.position.y > 9.0  && current_state.pose.position.y < 10.5)
         {
           mode =mode_change(mode);
        
         } 

     }

       
      if( mode == 1)

      {
        pos.pose.position.x =  10;//0.001*some_object.position_x;
        pos.pose.position.y =  0;//0.001*some_object.position_y;
        pos.pose.position.z = 10.0;//0.001*some_object.position_z;       
    
       

       if (current_state.pose.position.x > 9.0  && current_state.pose.position.x < 10.5)
         {
           mode =mode_change(mode);
        
         } 



      }
       

        if( mode == 2)

       {
        pos.pose.position.x =   0;//0.001*some_object.position_x;
        pos.pose.position.y =  -10;//0.001*some_object.position_y;
        pos.pose.position.z = 10.0;//0.001*some_object.position_z;       
    

        if (current_state.pose.position.y < -9.0  && current_state.pose.position.y > -10.5)
         {
           mode =mode_change(mode);
        
         } 
     

       }

      
      if (mode == 3)
      {
        
        pos.pose.position.x =   -10;//0.001*some_object.position_x;
        pos.pose.position.y =   0;//0.001*some_object.position_y;
        pos.pose.position.z = 10.0;//0.001*some_object.position_z; 

       if (current_state.pose.position.x < -9.0  && current_state.pose.position.x > -10.5)
         {
           mode =mode_change(mode);
        
         } 

      }


    if (mode == 4)
      {
        
        pos.pose.position.x =  0;  //0.001*some_object.position_x;
        pos.pose.position.y = 10.0;  //0.001*some_object.position_y;
        pos.pose.position.z = 10.0;  //0.001*some_object.position_z; 
   
         if (current_state.pose.position.y > 9.0  && current_state.pose.position.y < 10.5)
            {
                mode =mode_change_recusive(mode);
           
            } 
      
 
         
      }


/*

if (mode == 5)
      {
        
        pos.pose.position.x =   10;//0.001*some_object.position_x;
        pos.pose.position.y =   0;//0.001*some_object.position_y;
        pos.pose.position.z = 10.0;//0.001*some_object.position_z; 
   

    
      }

*/





       dm_pub.publish(pos); // Publish the topic 

       ros::spinOnce();
       count++;
       loop_rate.sleep();

       debugCount++;
       
   std::cout<<"debugCount "<<debugCount<<std::endl;
   }
   
      
   return 0;
}
