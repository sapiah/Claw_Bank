#include "NXTServo-lib-UW.c"

/* Sets the claw back to original position*/
void origin()
{
	while(nMotorEncoder[motorA] > 0)
		motor[motorA] = -15;
	motor[motorA] = 0;
	while(nMotorEncoder[motorB] < 0)
		motor[motorB] = 15;
	motor[motorB] = 0;
}

/*Function used to control the Servo motor. Helps in grabbing the prices contained in the lower level using the Mechanical Claw*/
void clawGrab()
{
	displayString(0, "Grabbing...");

	//open the claw
	setGripperPosition(S1, 3, 70);

  //drop the claw down
  setServoSpeed(S1, 1, 20, -20, 9);
  wait1Msec(3800);
  setServoSpeed(S1, 1, 0, -20, 9);

  //close the claw
  setGripperPosition(S1, 3, 0);
	wait1Msec(2000);

  //lift the claw
  setServoSpeed(S1, 1, -20, -20, 9);
  wait1Msec(4300);
  setServoSpeed(S1, 1, 0, -20, 9);
	eraseDisplay();
  origin();

  //open claw
  setGripperPosition(S1, 3, 70);
}

*/ Use to remotely control the claws in a 2-Directional nature (x,y) using colour sensors*/
void joystic(int & timeLeft)
{
	time1[T1] = 0;
	while(SensorValue[S3] == 0 && time1[T1] < 30000)
	{
			timeLeft = (30000-time1[T1])/1000;
			displayString(0, "Time left: %d", timeLeft);
	  	if(SensorValue[S2] == BLACKCOLOR && nMotorEncoder[motorA] < 485)
			{
				motor[motorA] = 15;
				motor[motorB] = 0;
			}
		  else if(SensorValue[S2] == YELLOWCOLOR && nMotorEncoder[motorA] > 0)
			{
				motor[motorA] = -15;
				motor[motorB] = 0;
			}
			else if(SensorValue[S2] == REDCOLOR && nMotorEncoder[motorB] > -450)
			{
				motor[motorB] = -15;
				motor[motorA] = 0;
			}
			else if(SensorValue[S2] == GREENCOLOR && nMotorEncoder[motorB] <= 0)
			{
				motor[motorB] = 15;
				motor[motorA] = 0;
			}
			else
			{
			motor[motorA] = 0;
			motor[motorB] = 0;
			}
			//displayString(3, "%d", nMotorEncoder[motorA]);
			//displayString(4, "%d", nMotorEncoder[motorB]);
	}
	motor[motorA] = 0;
	motor[motorB] = 0;
	eraseDisplay();
}

/*Plays music files based on different conditions*/
void music(const string sound)
{
	if (sound == "money")
		playSoundFile("louder better.rso");
	else if (sound == "chaching")
		playSoundFile("cha-ching.rso");
	else if (sound == "clap")
		playSoundFile("clap.rso");
}

/*Displays a single joke on the screen by randomly sellecting one from the list*/
void jokes()
{
const int max = 8;
string joke1[max];
joke1[0]= "#titanic{";
joke1[1]= "HIDE AND SEEK CHAMP:";
joke1[2]= "[\"hip\",\"hip\"]";
joke1[3]= "Why did the ";
joke1[4]= "What do you call a";
joke1[5]= "Helen Keller";
joke1[6]= "Where did mary go";
joke1[7]= "The baseball was";

string joke2[max];
joke2[0]= "float: none;";
joke2[1]= ";";
joke2[2]= "...";
joke2[3]= "integer drown?";
joke2[4]= "kid with no arms &";
joke2[5]= "walks in a bar...";
joke2[6]= "after the explosion?";
joke2[7]= "getting bigger...";

string joke3[max];
joke3[0]= "}";
joke3[1]= "SINCE 1958";
joke3[2]= "array";
joke3[3]= "it couldn't float!";
joke3[4]= "legs & eyes:Names";
joke3[5]= "table, & a chair.";
joke3[6]= "Everywhere!!!";
joke3[7]= "& then it hit me.";

int x = random(max);
displayString(0, "%s", joke1[x]);
displayString(1, "%s", joke2[x]);
displayString(2, "%s", joke3[x]);
wait1Msec(10000);
eraseDisplay();
}

/*Uses sensor to detect coin input to run the program and the claw machine*/
bool coinDetected()
{
	if (SensorValue[S4] < 50)
		return false;
	return true;
}

task main()
{
	SensorType[S1] = sensorI2CCustom9V;
	SensorType[S2] = sensorColorNxtFULL;
	SensorType[S3] = sensorTouch;
	SensorType[S4] = sensorLightActive;
	int coins = 0;
	nVolume = 3;

	while (nAvgBatteryLevel > 1000)
	{
		music("money");
		wait1Msec(1000);

		while (!coinDetected())
		{
			displayString(0, "Enter a coin");
			displayString(2, "Total coins: %d", coins);
		}

		music("chaching");
		wait1Msec(1000);
		coins++;
		displayString(2, "Total coins: %d", coins);
		int timeLeft;

		nMotorEncoder[motorA] = 0;
		nMotorEncoder[motorB] = 0;

		joystic(timeLeft);
		clawGrab();
		origin();

		music("clap");
		wait1Msec(1000);

		jokes();
		wait1Msec(2500);
	}
}
