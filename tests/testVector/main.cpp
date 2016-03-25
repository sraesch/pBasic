#include <pBasic/concurrent_vector.hpp>

#include <string>
#include <iostream>
#include <atomic>

namespace Results
{
    std::atomic<std::size_t>        numMoves(0);
    std::atomic<std::size_t>        numCopies(0);
    std::atomic<std::size_t>        numCreated(0);
    std::atomic<std::size_t>        numDestroyed(0);

    void reset()
    {
        numMoves = 0;
        numCopies = 0;
        numCreated = 0;
        numDestroyed = 0;
    }

    void show(std::ostream &out = std::cout)
    {
        out << "[RESULTS]: numMoves=" << numMoves << ", numCopies=" << numCopies
            << ", numCreated=" << numCreated << ", numDestroyed=" << numDestroyed << std::endl;
    }
}

struct Element
{
    static const std::size_t INVALID_ID = 0xFFFFFFFF;

    Element()
        : id(INVALID_ID)
    {
        Results::numCreated++;
    }

    Element(const std::size_t &id_)
        : Element()
    {
        this->id = id_;
    }

    Element(const Element &e)
        : Element(e.id)
    {
        Results::numCopies++;
    }

    Element(Element &&e)
        : Element()
    {
        this->id = std::move(e.id);
        e.id = INVALID_ID;
        Results::numMoves++;
    }

    ~Element()
    {
        if(id != INVALID_ID)
            Results::numDestroyed++;
    }

    std::size_t             id;
};

bool serialTestPushBack()
{
    // test push back using copy constructor
    {
        std::cout << "[INFO]: Serial: push_back with copy constructor...\n";

        pBasic::concurrent_vector<Element>  elements;
        Results::reset();

        for (std::size_t i = 0; i < 10; ++i)
        {
            const Element e(i);
            elements.push_back(e);
        }

        if (!(Results::numCopies == 10 && Results::numDestroyed == 10))
        {
            std::cout << "[INFO]: Serial: push_back with copy constructor...FAILED!!!\n";
            return false;
        }
    }

    // test push back using move constructor
    {
        std::cout << "[INFO]: Serial: push_back with move constructor...\n";

        pBasic::concurrent_vector<Element>  elements;
        Results::reset();

        for (std::size_t i = 0; i < 10; ++i)
        {
            Element e(i);
            elements.push_back(std::move(e));
        }

        if (!(Results::numCopies == 0 && Results::numDestroyed == 0))
        {
            std::cout << "[INFO]: Serial: push_back with move constructor...FAILED!!!\n";
            return false;
        }
    }

    return true;
}

bool serialTestEmplaceBack()
{
    std::cout << "[INFO]: Serial: emplace_back...\n";

    {
        pBasic::concurrent_vector<Element>  elements;
        Results::reset();

        for (std::size_t i = 0; i < 10; ++i)
            elements.emplace_back(i);

        if (!(Results::numCopies == 0 && Results::numDestroyed == 0))
        {
            std::cout << "[INFO]: Serial: emplace_back...FAILED!!!\n";
            return false;
        }
    }

    if (!(Results::numDestroyed == 10))
    {
        std::cout << "[INFO]: Serial: emplace_back destructor...FAILED!!!\n";
        return false;
    }

    return true;
}

bool serialTests()
{
    if (!serialTestPushBack())
        return false;

    if (!serialTestEmplaceBack())
        return false;

    return true;
}

int main()
{
    // perform serial tests
    if (!serialTests())
        return -1;

    return 0;
}
