#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>

class game_message
{
public:
    enum { header_length = 4 };
    enum { max_body_length = 1024 };

    game_message() : body_length_(0)
    {
    }

    const char* data() const
    {
        return data_;
    }

    char* data()
    {
        return data_;
    }

    size_t length() const
    {
        return header_length + body_length_;
    }

    const char* body() const
    {
        return data_ + header_length;
    }

    char* body()
    {
        return data_ + header_length;
    }

    size_t body_length() const
    {
        return body_length_;
    }

    void body_length(size_t new_length)
    {
        body_length_ = new_length;
        if (body_length_ > max_body_length)
            body_length_ = max_body_length;
    }

    bool decode_header()
    {
        body_length_ = data_[0];
        body_length_ <<= 8;
        body_length_ |= data_[1];
        body_length_ <<= 8;
        body_length_ |= data_[2];
        body_length_ <<= 8;
        body_length_ |= data_[3];
        if (body_length_ > max_body_length)
        {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        data_[0] = (body_length_ >> 24) & 0xFF;
        data_[1] = (body_length_ >> 16) & 0xFF;
        data_[2] = (body_length_ >>  8) & 0xFF;
        data_[3] = (body_length_      ) & 0xFF;
    }

private:
    char data_[header_length + max_body_length];
    size_t body_length_;
};

typedef std::deque<game_message> game_message_queue;
