#include "stdafx.h"
#include "protobuf_dispatcher.h"

void discardProtobufMessage(google::protobuf::Message* message)
{
    ;
}

ProtobufDispatcher::ProtobufDispatcher()
    : defaultCallback_(discardProtobufMessage)
{

}

void ProtobufDispatcher::onMessage(google::protobuf::Message* message) const
{
    CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
    if (it != callbacks_.end())
    {
        it->second(message);
    }
    else
    {
        defaultCallback_(message);
    }
}
