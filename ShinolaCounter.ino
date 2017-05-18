#define MAX_SPEED 180
#define MAX_OPTO_COUNT 12

// PINS
int BottomMotorPin = 5;
int MiddleMotorPin = 4;
int TopMotorPin = 3;

int bottomOptoInterrupt = 12;
int bottomOptoIndex = 11;

int middleOptoInterrupt = 10;
int middleOptoIndex = 9;

int topOptoInterrupt = 8;
int topOptoIndex = 7;

// ControlValues
int destinationTopBottom = 0;
int destinationMiddle = 0;

int curTopPos = 0;
int curBottomPos = 0;
int curMiddlePos = 0;

int lastTopIndexValue = 0;
int lastMiddleIndexValue = 0;
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

int readMiddleOptoIndex()
{
	return digitalRead(middleOptoIndex);
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

void SetOptoIndex()
{
	int curBottomOptoIndex = readBottomOptoIndex();
	if(curBottomOptoIndex == 0 && lastBottomIndexValue == 1)
	{
		curBottomPos = 0;
	}
	lastBottomIndexValue = curBottomOptoIndex;

	int curTopOptoIndex = readTopOptoIndex();
	if(curTopOptoIndex == 0 && lastTopIndexValue == 1)
	{
		curTopPos = 0;
	}
	lastTopIndexValue = curTopOptoIndex;


	int curMiddleOptoIndex = readMiddleOptoIndex();
	if(curMiddleOptoIndex == 0 && lastMiddleIndexValue == 1)
	{
		curMiddlePos = 0;
	}
	lastMiddleIndexValue = curMiddleOptoIndex;
}

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

	if(curBottomPos == destinationTopBottom)
	{
		stopBottomMotor();
	}

	if(curMiddlePos == destinationMiddle)
	{
		stopMiddleMotor();
	}
}

void MoveToNextGroup()
{
	if(curTopPos == destinationTopBottom && curBottomPos == destinationTopBottom && curMiddlePos == destinationMiddle)
	{
		StopOrDecelerate();
		delay(5000);
		destinationTopBottom = random(1, MAX_OPTO_COUNT);
		destinationMiddle = destinationTopBottom;
		setMiddleMotorSpeed(110);
		setTopMotorSpeed(150);
		setBottomMotorSpeed(180);
	}
}

void setupOptos()
{
	pinMode(bottomOptoInterrupt, INPUT_PULLUP);
	pinMode(bottomOptoIndex, INPUT_PULLUP);

	pinMode(middleOptoInterrupt, INPUT_PULLUP);
	pinMode(middleOptoIndex, INPUT_PULLUP);

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
		Serial.print(" Middle Index: "); Serial.print(readMiddleOptoIndex()); Serial.print(" Middle Opto: "); Serial.println(readMiddleOpto());
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

	do
	{
		printOptoStates();
		
		if(readBottomOptoIndex() == 0 && readBottomOpto() == 0)
		{
			stopBottomMotor();
			bottomDone = 1;
		}
		if(readMiddleOptoIndex() == 0 && readMiddleOpto() == 0)
		{
			stopMiddleMotor();
			middleDone = 1;
		}
		if(readTopOptoIndex() == 0 && readTopOpto() == 0)
		{
			stopTopMotor();
			topDone = 1;
		}
	}while(bottomDone == 0 || middleDone == 0 || topDone == 0);
	
	curBottomPos = 0;
	curMiddlePos = 0;
	curTopPos = 0;

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
}