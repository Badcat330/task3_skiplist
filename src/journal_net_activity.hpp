////////////////////////////////////////////////////////////////////////////////
// Module Name:  journal_net_activity.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include journal_net_activity.h here, 'cause it leads to circular refs. !!!

#include <fstream>

//==============================================================================
// class JournalNetActivity
//==============================================================================

template <int numLevels>
void JournalNetActivity<numLevels>::parseLog(const std::string& fullpath)
{
    // пытаемся открыть файл для чтения
    std::ifstream fin(fullpath);    // opens the file
    if (!fin)
        throw std::logic_error("Couldn't open file " + fullpath);
    
    parseLogFromStream(fin);
}

template <int numLevels>
void JournalNetActivity<numLevels>::parseLogFromStream(std::istream& in)
{
    TimeStamp timestamp;            // dummy
    NetActivity netactivity;        // dummy

    while (in)
    {
        in >> timestamp;
        if (!in)
            break;

        in >> netactivity.user;
        if (!in)
            break;

        in >> netactivity.host;
        if (!in)
            break;

        _journal.insert(netactivity, timestamp);
    }
}

//------------------------------------------------------------------------------

template <int numLevels>
void JournalNetActivity<numLevels>::dumpJournal(std::ostream& out)
{
    typename NetActivityList::Node* prehead = _journal.getPreHead();
    typename NetActivityList::Node* run = prehead;

    // prehead is placed before the first and after the last element
    // So it serves two roles.
    while (run->next != prehead)
    {
        run = run->next;
        out << run->key;
        out << " ";
        out << run->value;
    }
}

//------------------------------------------------------------------------------

template <int numLevels>
void JournalNetActivity<numLevels>::outputSuspiciousActivities(
        const std::string& hostSuspicious,
        const TimeStamp& timeFrom,
        const TimeStamp& timeTo,
        std::ostream& out) const
{
    if(timeFrom > timeTo)
    {
        throw std::invalid_argument("Wrong from and to arguments!");
    }

    auto startNode = _journal.findLastLessThan(timeFrom);
    auto endNode = _journal.findFirst(timeTo);

    if(startNode == nullptr || endNode == nullptr)
    {
        out << "";
        return;
    }

    while (startNode != endNode->next){
        if(hostSuspicious == startNode->value.host)
        {
            out << startNode->key << " " << startNode->value.user << " " << startNode->value.host << std::endl;
        }
        startNode = startNode->next;
    }
}