/** 
 *  TaskScheduler Test
 *
 *  Initially only tasks 1 and 2 are enabled
 *  Task1 runs every 2 seconds 10 times and then stops
 *  Task2 runs every 3 seconds indefinitely
 *  Task1 enables Task3 at its first run
 *  Task3 run every 5 seconds
 *  Task1 disables Task3 on its last iteration and changed Task2 to run every 1/2 seconds
 *  At the end Task2 is the only task running every 1/2 seconds
 */
 
 
#include <TaskScheduler.h>

// Callback methods prototypes
void t1Callback();

//Tasks
Task t1(2000, TASK_FOREVER, &t1Callback);

Scheduler runner;


void t1Callback() {
    Serial.print("t1: ");
    Serial.println(millis());
}


void setup () {
  Serial.begin(115200);
  Serial.println("Scheduler TEST");
  
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(t1);
  Serial.println("added t1");
  delay(5000);
  t1.enable();
  Serial.println("Enabled t1");

}


void loop () {
  runner.execute();
}
