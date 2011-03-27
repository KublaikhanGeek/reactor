#include "reactor.h"

int main(int argc, char ** argv)
{
    reactor::Reactor reactor;
    reactor.HandleEvents(10);
}