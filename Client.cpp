#include "Client.hpp"

void Client::buildResponse()
{

}

void Client::updateTime()
{
	_last_time = time(NULL);
}

void Client::clearClient()
{

}

time_t Client::getLastTime()
{
	return _last_time;
}