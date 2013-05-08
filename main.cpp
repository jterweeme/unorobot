class PanTilt
{
public:
    PanTilt();
};

class Robot
{
public:
    Robot();
private:
    PanTilt pt;
};

Robot::Robot()
{
}

int main()
{
    Robot robot;
    return 0;
}


