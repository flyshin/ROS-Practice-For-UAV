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

int* waypoint_mode(int *arr, int &mode, double x, double y);
   
//double* quternionTo(*angle);
double* quternionToeuler(double *current_angle);


double* eulerToquternion(double *quater_angle, double *Desired_angle);   // I would like to send euler angle
          
         
double* attitude_mode(int mode, double *Desired_angle);
         






int main(int argc, char **argv)
{
   ros::init(argc, argv, "pub_diamond_waypoint");
   ros::NodeHandle n;

   ros::Publisher dm_pub = n.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",100);
   ros::Subscriber state_sub = n.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose",100,state_cb);
  
   ros::Rate loop_rate(100);
   ros::spinOnce();

   
   int count = 1;
   int mode = 0; //waypoint mode

   //std::cout<<"dolgima" <<std::endl;

   geometry_msgs::PoseStamped pos;
  // int debugCount = 0
	
   while(ros::ok())
   {
       pos.header.stamp = ros::Time::now();
       pos.header.seq=count;
       pos.header.frame_id = 1;
    
     
      
       int arr[3];
       double current_angle[3];   
       double quater_angle[4];
       double Desired_angle[3];   // The desired angle we want to send

       quternionToeuler(current_angle);

       

      // std::cout<< angle[0] <<std::endl;
      // std::cout<< angle[1] <<std::endl;
       //std::cout<< angle[2] <<std::endl;

   
       waypoint_mode(arr,mode,current_state.pose.position.x, current_state.pose.position.y);

       
       attitude_mode(mode, Desired_angle); // attitude of  yaw is changed in each waypoint 
         


       pos.pose.position.x =  arr[0]; //0.001*some_object.position_x;
       pos.pose.position.y =  arr[1]; //0.001*some_object.position_y;
       pos.pose.position.z =  arr[2]; //0.001*some_object.position_z; 

       eulerToquternion(quater_angle,Desired_angle);

      
       pos.pose.orientation.w = quater_angle[0];
       pos.pose.orientation.x = quater_angle[1];
       pos.pose.orientation.y = quater_angle[2];
       pos.pose.orientation.z = quater_angle[3];
      
       

       //debugCount++;

       dm_pub.publish(pos); // Publish the topic 

       ros::spinOnce();
       count++;
       loop_rate.sleep();
      
/*
       std::cout<< quater_angle[ <<debugCount<<std::endl;
       std::cout<<"current_state.pose.position.x "<<current_state.pose.position.x<<std::endl;
       //std::cout<<"current_state.pose.position.y "<<current_state.pose.position.y<<std::endl;  */
   }
   
   
      
   return 0;
}








// definition



void state_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{

//std::cout<<"nada" <<std::endl;
 
 current_state =  *msg;
}


int* waypoint_mode(int *arr, int &mode, double x, double y )
    {
      
      

       if (mode == 0)
       {
          arr[0] = 0;
          arr[1] = 10.0;
          arr[2] = 10.0;

    
        
        
          if (y > 9.5  && y < 10.5)
          {
           mode = mode_change(mode);
          } 

       }


       if (mode == 1)
         {
          

          arr[0] =  10.0;
          arr[1] =  0;
          arr[2] = 10.0;

          if (x > 9.5  && x < 10.5)
          {
           mode = mode_change(mode);
          } 



       }





       if (mode == 2)
       {


          arr[0] = 0;
          arr[1] = -10.0;
          arr[2] = 10.0;

          if (y <-9.5  && y  > -10.5)
          {
           mode = mode_change(mode);
          } 



       }


       if (mode == 3)
       {


          arr[0] = -10.0;
          arr[1] =  0;
          arr[2] = 10.0;

          if (x <-9.5  && x  > -10.5)
          {
           mode = mode_change(mode);
          } 

       }


       

      if (mode == 4)
       {


          arr[0] = 0;
          arr[1] =  10.0;
          arr[2] = 10.0;

        if (y >9.5  && y  < 10.5)
          {
           mode = mode_change_recusive(mode);
          } 


       }





    	return  arr;
    }
    



int mode_change_recusive(int mode1)
{
	int mode =mode1-3;

	return mode;
}




int mode_change(int mode)
{
   mode = mode+1;
    
   return mode;
}



//double* quternionTo(*angle);


 double* quternionToeuler(double *current_angle)
       {
           
           //Current State

       	double x = current_state.pose.orientation.x;    
       	double y = current_state.pose.orientation.y;
       	double z = current_state.pose.orientation.z;
       	double w = current_state.pose.orientation.w;


           //Roll angle(x-axis rotation)

       	double sinr_cosp = 2*(w*x + y*z);
       	double cosr_cosp = 1 - 2*(x*x + y*y);
       	 current_angle[0] = std::atan2(sinr_cosp, cosr_cosp);  // roll angle

       	   //Pitch angle(y-axis rotation)
       	double sinp = 2*(w*y - z*x);
       	  
       	  if (std::abs(sinp) >= 1)
       	  	 current_angle[1] = std::copysign(M_PI/2, sinp); // use 90 degrees if out of range
       	  else
       	  	 current_angle[1] = std::asin(sinp);  //pitch angle

       	   // yaw ( y-axis rotation)

       	double siny_cosp = 2*(w*z + x*y);
       	double cosy_cosp = 1 -2*(y*y + z*z);
       	 current_angle[2] = std::atan2(siny_cosp, cosy_cosp); //yaw angle

     
         return current_angle;


       }




double* eulerToquternion(double *quater_angle, double *Desired_angle)   // I would like to send euler angle
          
          {
            
            // desired angle i would like to send
          /*
            double roll  = 0;  
            double pitch = 0;
            double yaw   = -1.5;
            */
            

            double cy = cos(Desired_angle[2]*0.5);
            double sy = sin(Desired_angle[2]*0.5);
            double cp = cos(Desired_angle[1]*0.5);
            double sp = sin(Desired_angle[1]*0.5); 
            double cr = cos(Desired_angle[0]*0.5);
            double sr = sin(Desired_angle[0]*0.5);

            
            // quaternion q;
             quater_angle[0] =cy*cp*cr + sy*sp*sr;
             quater_angle[1] = cy*cp*sr - sy*sp*cr;
             quater_angle[2] = sy*cp*sr + cy*sp*cr;
             quater_angle[3] = sy*cp*cr - cy*sp*sr;            

             return quater_angle;
          }






double* attitude_mode(int mode, double *Desired_angle)
          {
              
              if (mode == 0)
               {
              	Desired_angle[0]= 0;
                Desired_angle[1]= 0;
                Desired_angle[2]= 3.14/2;
               }


              if (mode ==1)
               {
                Desired_angle[0]= 0;
                Desired_angle[1]= 0;
                Desired_angle[2]= -3.14*(1/4);
               // Desired_angle[2]= 3.14 + 3.14*(3/4);
              
               }

              if (mode == 2)
               {
                Desired_angle[0]= 0;
                Desired_angle[1]= 0;
                //Desired_angle[2]= -3.14*(3/4);    
                Desired_angle[2]= -2.2;  
               } 

               if (mode == 3)
               {
                Desired_angle[0]= 0;
                Desired_angle[1]= 0;
                Desired_angle[2]=  2.2;    
               } 


               if (mode == 4)
               {
                Desired_angle[0]= 0;
                Desired_angle[1]= 0;
                //Desired_angle[2]= 3.14*(1/4);
                Desired_angle[2]= 1.5/2;    
               } 

              return Desired_angle;

          }

