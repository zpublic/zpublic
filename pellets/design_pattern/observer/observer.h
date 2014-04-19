#pragma once

class Observable;
class Observer
{
public:
    virtual ~Observer()
    {

    }

    virtual void update(Observable* p, void* lpData) = 0;
};