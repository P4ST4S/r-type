/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PackageParser
*/

#pragma once

#include "CrossPlatformNetwork.hpp"
#include "Package.hpp"

#include <deque>
#include <cstdint>
#include <memory>

/**
 ** @brief Get bytes and create package when there's enough data
 **
 */
class PackageParser {
protected:
    typedef enum parseStatus_u {
        WAIT_HEADER,
        INCOMPLETE_HEADER,
        INCOMPLETE_BODY,
    } parseStatus_t;
    parseStatus_t _status = WAIT_HEADER;
    cpn::Buffer _partBuf;
    Package _partPkg;
    std::deque<Package> _completePkgs;
    BodyFactory &_bodyFactory;

public:
    PackageParser(BodyFactory &_bodyFactory);
    PackageParser(const PackageParser &other);
    PackageParser(PackageParser &&other);
    ~PackageParser();

    PackageParser operator=(const PackageParser& other) {
        if (this != &other) {
            this->copy(other);
        }
        return (*this);
    }

    PackageParser& operator=(const PackageParser&& other) {
        if (this != &other) {
            this->_status = std::move(other._status);
            this->_partBuf = std::move(other._partBuf);
            this->_partPkg = std::move(other._partPkg);
            this->_completePkgs = std::move(other._completePkgs);
        }
        return (*this);
    }

    void copy(const PackageParser& other);

    /**
     ** @brief Add bytes to a part package until there's enough data to create a valid package.
     **
     ** @param data
     ** @return size_t
     */
    size_t writeBytesIn(const std::deque<uint8_t> &data);

    /**
     ** @brief Force the class to consider the newly added bytes as a new package.
     ** Destroy the part package that was currently in construction.
     ** Add bytes to a buffer until there's enough data to create a valid package.
     **
     ** @param data
     ** @return size_t
     */
    size_t writeBytesInReInit(const std::deque<uint8_t> &data);

    /**
     ** @brief Destroy the currently constructed part package.
     **
     */
    void destroyPartPackage(void);

    /**
     ** @brief Empty the list of already valid packages
     **
     */
    void destroyCompletePackages(void);

    /**
     ** @brief Call destroyPartPackage and destroyCompletePackages.
     **
     */
    void destroyAllPackages(void);

    /**
     ** @brief Get all the complete Packages object
     **
     ** @param nbPkg Number of packages to get. If 0, get all the packages.
     ** @return std::deque<std::unique_ptr<Package>>
     */
    std::deque<Package> getPackages(size_t nbPkg = 0);

    /**
     ** @brief Get all the complete Packages object
     **
     ** @return parseStatus_t
     */
    parseStatus_t getParsingStatus(void);

    std::deque<uint8_t> serializePackages(size_t nbPkg = 0);

    static std::deque<Package> extractPackages(std::deque<Package> &pkgs, const std::vector<CommandType_t> &whiteList);

protected:

    void parseData(void);
    void findHeader(void);
    void parseHeader(void);
    void parseBody(void);

private:
};
