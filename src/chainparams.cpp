// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The SLING developers
// Copyright (c) 2017 The Sling developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "genesis.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "chainparamsseeds.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("00000328f9124ad164c2a7e742c97a9dfa1e91be174d7e54c8181fc123c5da75"))
    ;
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1501986576,     // * UNIX timestamp of last checkpoint block
    0,              // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
    1440            // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x00000d8c3f4fd81e89467dbccba05afe018d978d12c7207e884ae983f9bb5f19"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1501987810,
    0,
    1440};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1501985300,
    0,
    1440};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x13;
        pchMessageStart[1] = 0x37;
        pchMessageStart[2] = 0x17;
        pchMessageStart[3] = 0x71;
        vAlertPubKey = ParseHex("048216a20abab9c0edb9a813328bbc4dad2082dda77c4b990bccbade46a8a331928c3a165aa4bd8ef1d8e1b9c9660da46dd6371eed67f92ec711d2e0cbfb13b47e");
        nDefaultPort = 31135;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Sling starting difficulty is 1 / 2^20
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Sling: 1 minute
        nTargetSpacing = 1 * 60;  // Sling: 1 minute
        nLastPOWBlock = std::numeric_limits<int>::max();
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 615800;
        startNewChain = false;

        genesis = CreateGenesisBlock(1501986576, 1135886, Convert_UintToArith256_GetCompact(bnProofOfWorkLimit), 1, (1.337 * COIN));
        if(startNewChain == true) {
            MineGenesis(genesis, bnProofOfWorkLimit, true);
        }

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000328f9124ad164c2a7e742c97a9dfa1e91be174d7e54c8181fc123c5da75"));
        assert(genesis.hashMerkleRoot == uint256("0xb8fcc02e857b1b1e1acd838130dcdca834f32b6fb28ef4c926e5d7b55d4f6229"));

        vSeeds.push_back(CDNSSeedData("slingmarket.net", "seed.slingmarket.net"));     // Primary DNS Seeder from slingmarket.net
        
        base58Prefixes[PUBKEY_ADDRESS] = boost::assign::list_of(63).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS] = boost::assign::list_of(85).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]     = boost::assign::list_of(153).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x04)(0x88)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false; //TODO: CryptoDJ, change back to true
        fAllowMinDifficultyBlocks = true; //TODO: CryptoDJ, change back to false
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "048216a20abab9c0edb9a813328bbc4dad2082dda77c4b990bccbade46a8a331928c3a165aa4bd8ef1d8e1b9c9660da46dd6371eed67f92ec711d2e0cbfb13b47e";
        strCoinMixPoolDummyAddress = "ShAEbDNFkjjtTLykrGDJ36SU2Hwroxf3P6";
        nStartMasternodePayments = 1501965300; //Sat, 2017-08-05 20:35:00 GMT
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x13;
        pchMessageStart[1] = 0x37;
        pchMessageStart[2] = 0x1f;
        pchMessageStart[3] = 0x31;
        vAlertPubKey = ParseHex("048216a20abab9c0edb9a813328bbc4dad2082dda77c4b990bccbade46a8a331928c3a165aa4bd8ef1d8e1b9c9660da46dd6371eed67f92ec711d2e0cbfb13b47e");
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Sling starting difficulty is 1 / 2^20
        nDefaultPort = 31136;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Sling: 1 minute
        nTargetSpacing = 1 * 60;  // Sling: 1 minute
        nLastPOWBlock = std::numeric_limits<int>::max();
        nMaturity = 15;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        startNewChain = false;

        genesis = CreateGenesisBlock(1501987810, 929409, Convert_UintToArith256_GetCompact(bnProofOfWorkLimit), 1, (1.337 * COIN));
        if(startNewChain == true) {
            MineGenesis(genesis, bnProofOfWorkLimit, true);
        }

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000d8c3f4fd81e89467dbccba05afe018d978d12c7207e884ae983f9bb5f19"));
        assert(genesis.hashMerkleRoot == uint256("0xb8fcc02e857b1b1e1acd838130dcdca834f32b6fb28ef4c926e5d7b55d4f6229"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("slingmarket.net", "testnet.seed.slingmarket.net"));

        base58Prefixes[PUBKEY_ADDRESS] = boost::assign::list_of(125).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS] = boost::assign::list_of(196).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]     = boost::assign::list_of(239).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x04)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "048216a20abab9c0edb9a813328bbc4dad2082dda77c4b990bccbade46a8a331928c3a165aa4bd8ef1d8e1b9c9660da46dd6371eed67f92ec711d2e0cbfb13b47e";
        nStartMasternodePayments = 1501965300; //Sat, 2017-08-05 20:35:00 GMT
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x13;
        pchMessageStart[1] = 0x37;
        pchMessageStart[2] = 0x1f;
        pchMessageStart[3] = 0x31;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Sling: 1 day
        nTargetSpacing = 1 * 60;        // Sling: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1454124731;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12345;
        startNewChain = false;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 31137;
        //assert(hashGenesisBlock == uint256("0x")); TODO: Get new genesis

        vFixedSeeds.clear(); //! Reg Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Reg Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 31138;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
        startNewChain = false;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
