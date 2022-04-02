#!/usr/bin/env python3
# license removed for brevity
import rospy
from sensor_msgs.msg import JointState
from std_msgs.msg import Float64
from pynput import keyboard

# TODO: replace Float64 msg to sensor_msgs/JointState
# TODO: Make Add other keyboar handle keys to other joints

class DemoNode():
    def __init__(self):
        self.pub_joint_1 = rospy.Publisher('/modelo/rev2_position_controller/command', Float64, queue_size=1)
        self.pub_joint_2 = rospy.Publisher('/modelo/rev3_position_controller/command', Float64, queue_size=1)
        self.pub_joint_3 = rospy.Publisher('/modelo/rev4_position_controller/command', Float64, queue_size=1)
        self.pub_joint_4 = rospy.Publisher('/modelo/rev5_position_controller/command', Float64, queue_size=1)
        self.pub_joint_5 = rospy.Publisher('/modelo/rev6_position_controller/command', Float64, queue_size=1)
        self.pub_joint_6 = rospy.Publisher('/modelo/rev7_position_controller/command', Float64, queue_size=1)
        # TODO: implement the same idea to other joints as follows
        # self.pub_joint_2 = rospy.Publisher('/rrbot/joint_2/cmd', Float64, queue_size=1)
        # self.pub_joint_3 = rospy.Publisher('/rrbot/joint_3/cmd', Float64, queue_size=1)

        #use same timer callback to handle all publishers
        self.timer = rospy.Timer(rospy.Duration(0.1), self.get_input_CB)
        self.counter1 = 0
        self.counter2 = 0
        self.counter3 = 0
        self.counter4 = 0
        self.counter5 = 0
        self.counter6 = 0

    def demo_callback(self, timer):
        print("entering callback")
        msg = Float64()
        print("Reading the message correctly")
        msg.data = self.counter
        self.counter1 += 1
        rospy.loginfo("Publishing message {}".format(msg.data))
        self.pub_joint_1.publish(msg)
        self.pub_joint_2.publish(msg)
        self.pub_joint_3.publish(msg)
        self.pub_joint_4.publish(msg)
        self.pub_joint_5.publish(msg)
        self.pub_joint_6.publish(msg)
    def get_input_CB(self, timer):
        with keyboard.Events() as events:
                # TODO: Add exception of 
                event = events.get(0.1)
                if event is None:
                    pass  # print('You did not press a key within one second')
                else:
                    msg = Float64()
                    #print('Received Key {}'.format(event.key))
                    try:
                        #Joint 1
                        if event.key.char ==  "a":#keyboard/.#.keyboard.Key.esc:
                            self.counter1 += 0.05
                            msg.data = self.counter1
                            self.pub_joint_1.publish(msg)
                            #print(msg.data)
                        elif event.key.char ==  "z":#keyboard/.#.keyboard.Key.esc:
                            self.counter1 -= 0.05
                            msg.data = self.counter1
                            self.pub_joint_1.publish(msg)
                            #print(msg.data)
                        #Joint 2
                        elif event.key.char ==  "s":#keyboard/.#.keyboard.Key.esc:
                            self.counter2 += 0.05
                            msg.data = self.counter2
                            self.pub_joint_2.publish(msg)
                        elif event.key.char ==  "x":#keyboard/.#.keyboard.Key.esc:
                            self.counter2 -= 0.05
                            msg.data = self.counter2
                            self.pub_joint_2.publish(msg)
                        #Joint 3
                        elif event.key.char ==  "d":#keyboard/.#.keyboard.Key.esc:
                            self.counter3 += 0.05
                            msg.data = self.counter3
                            self.pub_joint_3.publish(msg)
                        elif event.key.char ==  "c":#keyboard/.#.keyboard.Key.esc:
                            self.counter3 -= 0.05
                            msg.data = self.counter3
                            self.pub_joint_3.publish(msg)  
                        #Joint 4
                        elif event.key.char ==  "f":#keyboard/.#.keyboard.Key.esc:
                            self.counter4 += 0.05
                            msg.data = self.counter4
                            self.pub_joint_4.publish(msg)
                        elif event.key.char ==  "v":#keyboard/.#.keyboard.Key.esc:
                            self.counter4 -= 0.05
                            msg.data = self.counter4
                            self.pub_joint_4.publish(msg)
                        #Joint 5
                        elif event.key.char ==  "g":#keyboard/.#.keyboard.Key.esc:
                            self.counter5 += 0.05
                            msg.data = self.counter5
                            self.pub_joint_5.publish(msg)
                        elif event.key.char ==  "b":#keyboard/.#.keyboard.Key.esc:
                            self.counter5 -= 0.05
                            msg.data = self.counter5
                            self.pub_joint_5.publish(msg)       
                        #Joint 6
                        elif event.key.char ==  "h":#keyboard/.#.keyboard.Key.esc:
                            self.counter6 += 0.05
                            msg.data = self.counter6
                            self.pub_joint_6.publish(msg)
                        elif event.key.char ==  "n":#keyboard/.#.keyboard.Key.esc:
                            self.counter6 -= 0.05
                            msg.data = self.counter6
                            self.pub_joint_6.publish(msg)       
                        else:
                            pass
                    except AttributeError:
                        pass

if __name__ == '__main__':
    rospy.init_node('teleop_keyboard')
    while not rospy.is_shutdown():
        try:
            DemoNode()
            print("\n")
            print("------------\n")
            print("Control the Arm\n")
            print("------------\n")
            print("a : increase joint 1 angle\n")
            print("z : decrease joint 1 angle\n")
            print("s : increase joint 2 angle\n")
            print("x : decrease joint 2 angle\n")
            print("d : increase joint 3 angle\n")
            print("c : decrease joint 3 angle\n")
            print("f : increase joint 4 angle\n")
            print("v : decrease joint 4 angle\n")
            print("g : increase joint 5 angle\n")
            print("b : decrease joint 5 angle\n")
            rospy.spin()
        except rospy.ROSInterruptException:
            print("exception thrown")
            pass