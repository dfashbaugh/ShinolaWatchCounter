#define MAX_SPEED 180
#define MAX_OPTO_COUNT 12

int max_top_speed = 180;
int min_top_speed = 100;
int top_speed = 150;

int bottomSpeed = 160;

int middleSpeed = 130;

// PINS
int BottomMotorPin = 5;
int MiddleMotorPin = 4;
int TopMotorPin = 3;

int bottomOptoInterrupt = 12;
int bottomOptoIndex = 11;

int middleOptoInterrupt = 10;

int topOptoInterrupt = 8;
int topOptoIndex = 7;

// ControlValues
int destinationTopBottom = 0;
int destinationMiddle = 0;

int curTopPos = 0;
int curBottomPos = 0;
int curMiddlePos = 0;

int lastTopIndexValue = 0;
int lastBottomIndexValue = 0;

int lastTopOptoValue = 0;
int lastBottomOptoValue = 0;
int lastMiddleOptoValue = 0;

int curTopSpeed = 0;
int curBottomSpeed = 0;
int curMiddleSpeed = 0;

void setMotorSpeed(int pin, int speed)
{
	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}

	analogWrite(pin, speed);
}

void setTopMotorSpeed(int speed)
{
	setMotorSpeed(TopMotorPin, speed);
}

void setMiddleMotorSpeed(int speed)
{
	setMotorSpeed(MiddleMotorPin, speed);
}

void setBottomMotorSpeed(int speed)
{
	setMotorSpeed(BottomMotorPin, speed);
}

void stopAllMotors()
{
	setTopMotorSpeed(0);
	setMiddleMotorSpeed(0);
	setBottomMotorSpeed(0);
}

void stopTopMotor()
{
	setTopMotorSpeed(0);
}

void stopBottomMotor()
{
	setBottomMotorSpeed(0);
}

void stopMiddleMotor()
{
	setMiddleMotorSpeed(0);
}

int readBottomOpto()
{
	return digitalRead(bottomOptoInterrupt);
}

int readBottomOptoIndex()
{
	return digitalRead(bottomOptoIndex);
}

int readMiddleOpto()
{
	return digitalRead(middleOptoInterrupt);
}

int readTopOpto()
{
	return digitalRead(topOptoInterrupt);
}

int readTopOptoIndex()
{
	return digitalRead(topOptoIndex);
}

int incrementOptoPosition(int curPosition)
{
	curPosition++;
	if(curPosition > MAX_OPTO_COUNT)
	{
		curPosition = 1;
	}

	return curPosition;
}

int topIndexCounts = 0;
int botIndexCounts = 0;
int midIndexCounts = 0;
int indexCountThreshold = 500;
void SetOptoIndex()
{
	int curBottomOptoIndex = readBottomOptoIndex();
	if(curBottomOptoIndex == 0)
	{
		botIndexCounts++;
	}
	else
	{
		botIndexCounts = 0;
	}
	
	if(botIndexCounts == indexCountThreshold)
	{
		if(curBottomPos != 0 & curBottomPos != 12)
		{
			Serial.print("Potential miscount on bottom. Hit index, but at "); Serial.println(curBottomPos);
		}
		curBottomPos = 0;
	}

	int curTopOptoIndex = readTopOptoIndex();
	if(curTopOptoIndex == 0)
	{
		topIndexCounts++;
	}
	else
	{
		topIndexCounts = 0;
	}
	
	if(topIndexCounts == indexCountThreshold)
	{
		if(curTopPos != 0 & curTopPos != 12)
		{
			Serial.print("Potential miscount on top. Hit index, but at "); Serial.println(curTopPos);
		}
		curTopPos = 0;
	}
}

int topCounts = 0;
int botCounts = 0;
int midCounts = 0;
int countThreshold = 20;
void CheckOptos()
{
	int curTopOpto = readTopOpto();
	if(curTopOpto == 0 && lastTopOptoValue == 0)
	{
		topCounts++;
	}

	if(curTopOpto == 1)
	{
		topCounts = 0;
	}

	lastTopOptoValue = curTopOpto;

	if(topCounts == countThreshold)
	{
		curTopPos = incrementOptoPosition(curTopPos);
	}



	int curMiddleOpto = readMiddleOpto();
	if(lastMiddleOptoValue == 0 && curMiddleOpto == 0)
	{
		midCounts++;
	}

	if(curMiddleOpto == 1)
	{
		midCounts = 0;
	}

	lastMiddleOptoValue = curMiddleOpto;

	if(midCounts == countThreshold)
	{
		curMiddlePos = incrementOptoPosition(curMiddlePos);
	}



	int curBottomOpto = readBottomOpto();
	if(lastBottomOptoValue == 0 && curBottomOpto == 0)
	{
		botCounts++;
	}

	if(curBottomOpto == 1)
	{
		botCounts = 0;
	}

	lastBottomOptoValue = curBottomOpto;

	if(botCounts == countThreshold)
	{
		curBottomPos = incrementOptoPosition(curBottomPos);
	}

	SetOptoIndex();
}

/*
void CheckOptos()
{
	int curTopOpto = readTopOpto();
	if(lastTopOptoValue == 1 && curTopOpto == 0)
	{
		curTopPos = incrementOptoPosition(curTopPos);
		Serial.print("Cur Top Pos: ");
		Serial.print(curTopPos);
		Serial.print(" Destination Top Pos: ");
		Serial.println(destinationTopBottom);
	}
	lastTopOptoValue = curTopOpto;

	int curMiddleOpto = readMiddleOpto();
	if(lastMiddleOptoValue == 1 && curMiddleOpto == 0)
	{
		curMiddlePos = incrementOptoPosition(curMiddlePos);
		Serial.print("Cur Middle Pos: ");
		Serial.print(curMiddlePos);
		Serial.print(" Destination Middle Pos: ");
		Serial.println(destinationMiddle);
	}
	lastMiddleOptoValue = curMiddleOpto;

	int curBottomOpto = readBottomOpto();
	if(lastBottomOptoValue == 1 && curBottomOpto == 0)
	{
		curBottomPos = incrementOptoPosition(curBottomPos);
		Serial.print("Cur Bottom Pos: ");
		Serial.print(curBottomPos);
		Serial.print(" Destination Bottom Pos: ");
		Serial.println(destinationTopBottom);
	}
	lastBottomOptoValue = curBottomOpto;

	SetOptoIndex();
}
*/

int CalculateDeceleratePosition(int destination)
{
	int decelerate = destination - 1;

	if(decelerate < 0)
	{
		decelerate == MAX_OPTO_COUNT;
	}
}

void StopOrDecelerate()
{
	if(curTopPos == destinationTopBottom)
	{
		stopTopMotor();
	}
	else
	{
		setTopMotorSpeed(top_speed);
	}

	if(curBottomPos == destinationTopBottom)
	{
		stopBottomMotor();
	}
	else
	{
		setBottomMotorSpeed(bottomSpeed);
	}

	if(curMiddlePos == destinationMiddle)
	{
		stopMiddleMotor();
	}
	else
	{
		setMiddleMotorSpeed(middleSpeed);
	}
}

void ChooseNextPositionRelative(int maxDiff)
{
	int newVal = random(1, maxDiff);

	destinationTopBottom += newVal;

	if(destinationTopBottom > MAX_OPTO_COUNT)
	{
		destinationTopBottom = destinationTopBottom - MAX_OPTO_COUNT;
	} 

	//destinationMiddle = destinationTopBottom;

	newVal = random(1, maxDiff);
	destinationMiddle += newVal;

	if(destinationMiddle > MAX_OPTO_COUNT)
	{
		destinationMiddle = destinationMiddle - MAX_OPTO_COUNT;
	}
}

void MoveToNextGroup()
{
	if(curTopPos == destinationTopBottom && curBottomPos == destinationTopBottom && curMiddlePos == destinationMiddle)
	{
		StopOrDecelerate();
		delay(5000);
		ChooseNextPositionRelative(5);
		//destinationTopBottom = random(1, MAX_OPTO_COUNT);
		//destinationMiddle = destinationTopBottom;
		setMiddleMotorSpeed(middleSpeed);
		setTopMotorSpeed(top_speed);
		setBottomMotorSpeed(bottomSpeed);
	}
}

void setTopSpeed()
{
	int analogVal = analogRead(A0);

	top_speed = map(analogVal, 1024, 0, min_top_speed, max_top_speed);
}

void setupOptos()
{
	pinMode(bottomOptoInterrupt, INPUT_PULLUP);
	pinMode(bottomOptoIndex, INPUT_PULLUP);

	pinMode(middleOptoInterrupt, INPUT_PULLUP);

	pinMode(topOptoInterrupt, INPUT_PULLUP);
	pinMode(topOptoIndex, INPUT_PULLUP);
}

unsigned long lastMillis = 0;
void printOptoStates()
{
	if(millis() - lastMillis > 200)
	{
		lastMillis = millis();
		Serial.print("Bottom Index: "); Serial.print(readBottomOptoIndex()); Serial.print(" Bottom Opto: "); Serial.print(readBottomOpto());
		Serial.print(" Top Index: "); Serial.print(readTopOptoIndex()); Serial.print(" Top Opto: "); Serial.print(readTopOpto());
		Serial.print(" Middle Opto: "); Serial.println(readMiddleOpto());
	}
}

void Initialize()
{
	Serial.println("Run Motors");
	setMiddleMotorSpeed(255);
	setTopMotorSpeed(255);
	setBottomMotorSpeed(255);

	int bottomDone = 0;
	int topDone = 0;
	int middleDone = 0;

	int bottomAcc = 0;
	int middleAcc = 0;
	int topAcc = 0;

	int threshold = 500;

	do
	{
		printOptoStates();
		
		if(readBottomOptoIndex() == 0 && readBottomOpto() == 0)
		{
			bottomAcc++;
		}
		else
		{
			bottomAcc = 0;
		}

		if(readTopOptoIndex() == 0 && readTopOpto() == 0)
		{
			topAcc++;
		}
		else 
		{
			topAcc = 0;
		}

		if(bottomAcc > threshold)
		{
			stopBottomMotor();
			bottomDone = 1;
		}
		if(topAcc > threshold)
		{
			stopTopMotor();
			topDone = 1;
		}

	}while(bottomDone == 0 || middleDone == 0 || topDone == 0);
	
	curBottomPos = 1;
	curMiddlePos = 1;
	curTopPos = 1;

}

void setup() {
  // put your setup code here, to run once:
  Serial.println("Start");
  setupOptos();
  Initialize();
}

void loop() {
  //printOptoStates();
  // put your main code here, to run repeatedly:
  MoveToNextGroup();
  CheckOptos();
  StopOrDecelerate();
  setTopSpeed();
}