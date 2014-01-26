#ifndef __DATA_CACHED_H__
#define __DATA_CACHED_H__

class DataCached
{
    virtual bool loadFromDB() = 0;
    virtual bool saveToDB() = 0;
    virtual void loadFromMemCached() = 0;
    virtual void saveToMemCached() = 0;
};

#endif // !__DATA_CACHED_H__
