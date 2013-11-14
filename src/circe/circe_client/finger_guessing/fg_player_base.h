#ifndef __GAME_PLAYER_OBJECT_BASE
#define __GAME_PLAYER_OBJECT_BASE

#define INITIALIZE_LIFT_VALUE 10

class FGPlayerBase
{
public:
    FGPlayerBase();
    ~FGPlayerBase();

    void AddLift(int nLift);

    void SubLift(int nLift);

    int GetLift() const;

private:
    int m_nLift;
};

#endif