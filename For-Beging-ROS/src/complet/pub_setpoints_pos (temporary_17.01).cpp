#include <ros/ros.h>
#include <std_msgs/String.h> 
#include <stdio.h>
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Vector3Stamped.h"



  // Wapoint flight mode 


// declaration


geometry_msgs::PoseStamped current_state;
void state_cb(const geometry_msgs::PoseStamped::ConstPtr& msg);

int mode_change(int mode1);  // To change the waypoint mode

int mode_change_recusive(int mode1);

int* waypoint_mode(int *arr, int &mode1, int x, int y);
   





int main(int argc, char **argv)
{
   ros::init(argc, argv, "pub_setpoints");
   ros::NodeHandle n;

   ros::Publisher dm_pub = n.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",100);
   ros::Subscriber state_sub = n.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose",100,state_cb);
  
   ros::Rate loop_rate(100);
   ros::spinOnce();

   
   int count = 1;
   int mode = 0; 

   
	
		//PositionReciever qp;:
 		//Body some_object;
		//qp.connect_to_server();

   int debugCount = 0;

	
   while(ros::ok())
   {


       //std::cout<<"debugCount "<<debugCount<<std::endl;
   	   geometry_msgs::PoseStamped pos;
	   //some_object = qp.getStatus();
		// some_object.print();
		//printf("%f\n",some_object.position_x);
       pos.header.stamp = ros::Time::now();
       pos.header.seq=count;
       pos.header.frame_id = 1;
     
      
       pos.pose.orientation.x = 0;
       pos.pose.orientation.y = 0;
       pos.pose.orientation.z = 0;
       pos.pose.orientation.w = 1;
      
       int arr[3];
       

       waypoint_mode(arr,mode,current_state.pose.position.x, current_state.pose.position.y);

       
       pos.pose.position.x =  arr[0]; //0.001*some_object.position_x;
       pos.pose.position.y =  arr[1]; //0.001*some_object.position_y;
       pos.pose.position.z =  arr[2]; //0.001*some_object.position_z; 


       debugCount++;


       dm_pub.publish(pos); // Publish the topic 

       ros::spinOnce();
       count++;
       loop_rate.sleep();
       std::cout<<"debugCount "<<debugCount<<std::endl;
   }
   
   
      
   return 0;
}








// definition



void state_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{

 current_state =  *msg;
}


int* waypoint_mode(int *arr, int &mode1, int x, int y)
    {



       if (mode1 == 0)
       {
          arr[0] = 0;
          arr[1] = 10.0;
          arr[2] = 10.0;
        
          if (y > 9.0  && y < 11.0)
          {
           mode1 = mode_change(mode1);
          } 

       }


       if (mode1 == 1)
         {


          arr[0] =  10.0;
          arr[1] =  0;
          arr[2] = 10.0;

          if (x > 9.0  && x < 11.0)
          {
           mode1 = mode_change(mode1);
          } 



       }





       if (mode1 == 2)
       {


          arr[0] = 0;
          arr[1] = -10.0;
          arr[2] = 10.0;

          if (y <-9.0  && y  > -11.0)
          {
           mode1 = mode_change(mode1);
          } 



       }


       if (mode1 == 3)
       {


          arr[0] = -10.0;
          arr[1] =  0;
          arr[2] = 10.0;

          if (x <-9.0  && x  > -11.0)
          {
           mode1 = mode_change(mode1);
          } 

       }


       

      if (mode1 == 4)
       {


          arr[0] = 0;
          arr[1] =  10.0;
          arr[2] = 10.0;

       }





    	return  arr;
    }

/*

int mode_change_recusive(int mode1)
{
	int mode =mode1-3;

	return mode;
}


*/

int mode_change(int mode)
{
   mode = mode+1;
    
   return mode;
}
