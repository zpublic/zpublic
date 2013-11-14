#ifndef __GAME_PLAYER_OBJECT_BASE
#define __GAME_PLAYER_OBJECT_BASE

#define INITIALIZE_LIFT_VALUE 10

class CPlayerGameBase
{
public:
    CPlayerGameBase();
    ~CPlayerGameBase();

    void AddLift(UINT nLift);

    void SubLift(UINT nLift);

private:
    UINT m_nLift;
};

#endif