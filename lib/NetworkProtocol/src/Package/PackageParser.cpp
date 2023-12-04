/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PackageParser
*/

#include "PackageParser.hpp"

PackageParser::PackageParser(BodyFactory &_bodyFactory) :
_bodyFactory(_bodyFactory)
{
}

PackageParser::PackageParser(const PackageParser &other) :
_bodyFactory(other._bodyFactory)
{
    this->copy(other);
}

PackageParser::PackageParser(PackageParser &&other) :
_status(std::move(other._status)),
_partBuf(std::move(other._partBuf)),
_partPkg(std::move(other._partPkg)),
_completePkgs(std::move(other._completePkgs)),
_bodyFactory(other._bodyFactory)
{
}

PackageParser::~PackageParser()
{
}

void PackageParser::copy(const PackageParser& other)
{
    this->_status = other._status;
    this->_partBuf = other._partBuf;
    this->_partPkg = other._partPkg;
    this->_completePkgs = other._completePkgs;
}

size_t PackageParser::writeBytesIn(const std::deque<uint8_t> &data)
{
    size_t bytes = this->_partBuf.writeIn(data);

    do {
        this->parseData();
    } while (this->_status == WAIT_HEADER && this->_partBuf.size() > 0);
    return (bytes);
}

size_t PackageParser::writeBytesInReInit(const std::deque<uint8_t> &data)
{
    this->destroyPartPackage();
    return (this->writeBytesIn(data));
}

void PackageParser::destroyPartPackage(void)
{
    this->_partBuf.clear();
    this->_status = WAIT_HEADER;
}

void PackageParser::destroyCompletePackages(void)
{
    this->_completePkgs.resize(0);
}

void PackageParser::destroyAllPackages(void)
{
    this->destroyPartPackage();
    this->destroyCompletePackages();
}

std::deque<Package> PackageParser::getPackages(size_t nbPkg)
{
    std::deque<Package> pkgs;

    if (nbPkg == 0 || nbPkg > this->_completePkgs.size())
        nbPkg = this->_completePkgs.size();
    for (size_t i = 0; i < nbPkg; i++) {
        pkgs.push_back(std::move(this->_completePkgs.front()));
        this->_completePkgs.pop_front();
    }
    return (std::move(pkgs));
}

PackageParser::parseStatus_t PackageParser::getParsingStatus(void)
{
    return (this->_status);
}

void PackageParser::parseData(void)
{
    this->findHeader();
    this->parseHeader();
    this->parseBody();
}

void PackageParser::findHeader(void)
{
    size_t i = 0;
    size_t bufSize = this->_partBuf.size();

    if (this->_status == WAIT_HEADER && bufSize >= MAGIC_NUMBER_SIZE) {
        for (i = 0; i <= bufSize - MAGIC_NUMBER_SIZE; i++) {
            if (memcmp(DEQUE_DATA(this->_partBuf.get()) + i, MAGIC_NUMBER, MAGIC_NUMBER_SIZE) == 0) {
                this->_status = INCOMPLETE_HEADER;
                break;
            }
        }
        this->_partBuf.readIn(i);
    }
}

void PackageParser::parseHeader(void)
{
    size_t bodySize;
    std::deque<uint8_t> data;

    if (this->_status == INCOMPLETE_HEADER && this->_partBuf.size() >= HEADER_SIZE) {
        data = this->_partBuf.readIn(HEADER_SIZE);
        this->_partPkg.getHeader().deserialize(data);

        // std::cout << "Receive" << std::endl;
        // std::cout << this->_partPkg.getHeader() << std::endl;

        bodySize = this->_partPkg.getHeader().get()._bodySize;
        this->_partPkg.setBody(this->_bodyFactory.createBody(this->_partPkg.getHeader().get()._commandType));
        this->_partPkg.getHeader().get()._bodySize = bodySize;

        this->_status = INCOMPLETE_BODY;
    }
}


void PackageParser::parseBody(void)
{
    size_t bodySize = this->_partPkg.getHeader().get()._bodySize;

    if (this->_status == INCOMPLETE_BODY && this->_partBuf.size() >= bodySize) {
        this->_partPkg.getBody<IBody>().deserialize(this->_partBuf.readIn(bodySize));
        this->_partPkg.updateBodySize();
        this->_completePkgs.push_back(this->_partPkg);
        this->_status = WAIT_HEADER;
    }
}

std::deque<uint8_t> PackageParser::serializePackages(size_t nbPkg)
{
    std::deque<uint8_t> tmp;
    std::deque<uint8_t> data;
    std::deque<Package> pkgs = this->getPackages(nbPkg);

    for (size_t i = 0; i < pkgs.size(); i++) {
        tmp = pkgs[i].serialize();
        data.insert(data.end(), tmp.begin(), tmp.end());
    }
    return (std::move(data));
}

std::deque<Package> PackageParser::extractPackages(std::deque<Package> &pkgs, const std::vector<CommandType_t> &whiteList)
{
    Package pkg;
    std::deque<Package> extractedPkgs;
    size_t pkgsLen = pkgs.size();
    std::unordered_map<CommandType_t, bool> cmdMap;
    std::vector<CommandType_t>::const_iterator itCmd;

    for (itCmd = whiteList.begin(); itCmd != whiteList.end(); ++itCmd)
        cmdMap[*itCmd] = true;
    for (size_t i = 0; i < pkgsLen; i++) {
        pkg = pkgs.front();
        pkgs.pop_front();
        if (cmdMap.find(pkg.getHeader().get()._commandType) != cmdMap.end())
            extractedPkgs.push_back(std::move(pkg));
        else {
            pkgs.push_back(std::move(pkg));
        }
    }
    // std::cout << "Extracted " << extractedPkgs.size() << " packages | keeped: " << pkgs.size() << std::endl;
    return (extractedPkgs);
}
