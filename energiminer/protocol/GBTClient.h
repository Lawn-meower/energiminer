/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */
#ifndef JSONRPC_CPP_STUB_FARMCLIENT_H_
#define JSONRPC_CPP_STUB_FARMCLIENT_H_

#include <jsonrpccpp/client.h>
#include <iostream>

#include "energiminer/common/Log.h"
#include "MiningClient.h"

using namespace std;

class GBTClient : public MiningClient, jsonrpc::Client
{
public:
    GBTClient(jsonrpc::IClientConnector &conn, const std::string conbaseAddress, jsonrpc::clientVersion_t type = jsonrpc::JSONRPC_CLIENT_V1)
        : jsonrpc::Client(conn, type)
        , m_coinbaseAddress(conbaseAddress)
    {}

    Json::Value getBlockTemplate() throw (jsonrpc::JsonRpcException) override
    {
        auto params = Json::Value(Json::arrayValue);
        auto object = Json::Value(Json::objectValue);
        object["capabilities"] = Json::Value(Json::arrayValue);
        object["capabilities"].append("coinbasetxn");
        object["capabilities"].append("coinbasevalue");
        object["capabilities"].append("longpoll");
        object["capabilities"].append("workid");
        params.append(object);

        Json::Value result = this->CallMethod("getblocktemplate", params);
        if (result.isObject() ) {
            return result;
        } else {
            throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
    }

    energi::Work getWork() override
    {
        auto workGBT = getBlockTemplate();
        return energi::Work(workGBT, m_coinbaseAddress);
    }

    bool submit(const energi::Solution &solution) override
    {
        Json::Value params(Json::arrayValue);
        auto result1 = solution.getSubmitBlockData();
        params.append(result1);
        Json::Value result = this->CallMethod("submitblock", params);
        auto resultStr = result.toStyledString();
        resultStr.pop_back(); // includes a newline
        if (resultStr == "null")
        {
            cnote << "Block Accepted";
        }
        else if (resultStr == "inconclusive")
        {
            cnote << "Block too old";
        }
        else
        {
            cnote << "Block Rejected:" << resultStr;
        }
        return resultStr == "null";
    }

private:
    std::string m_coinbaseAddress;
};

#endif //JSONRPC_CPP_STUB_FARMCLIENT_H_
