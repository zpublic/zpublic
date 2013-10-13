#ifndef __NAME_VALUE_COLLECTION_H__
#define __NAME_VALUE_COLLECTION_H__


#include <string>
#include <map>

class NameValueCollection
{
public:
    struct ILT
    {
        bool operator() (const std::string& s1, const std::string& s2) const
        {
            return s1.compare(s2) < 0;
        }
    };

    typedef std::multimap<std::string, std::string, ILT> HeaderMap;
    typedef HeaderMap::iterator Iterator;
    typedef HeaderMap::const_iterator ConstIterator;

    NameValueCollection();
    NameValueCollection(const NameValueCollection& nvc);

    virtual ~NameValueCollection();

public:
    NameValueCollection& operator = (const NameValueCollection& nvc);
    const std::string& operator [] (const std::string& name) const;

    void swap(NameValueCollection& nvc);
    void set(const std::string& name, const std::string& value);	
    void add(const std::string& name, const std::string& value);
    const std::string& get(const std::string& name) const;
    const std::string& get(const std::string& name, const std::string& defaultValue) const;
    bool has(const std::string& name) const;
    ConstIterator find(const std::string& name) const;
    ConstIterator begin() const;
    ConstIterator end() const;
    bool empty() const;
    int size() const;
    void erase(const std::string& name);
    void clear();

private:
    HeaderMap _map;
};

inline void swap(NameValueCollection& nvc1, NameValueCollection& nvc2)
{
    nvc1.swap(nvc2);
}

#endif