#pragma once
#include "../Types.h"

#include <memory>
#include <functional>

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CWifiClientHandler;
using CWifiClientHandler_ptr = std::shared_ptr<CWifiClientHandler>;

class CWifiClientHandler
{
    using print_func_t = std::function<void(std::string)>;

  public:
    static CWifiClientHandler_ptr Create(CPresenter_ptr presenter);

    void HandleLine();

    void PrintStatus();
    void SetPrint(print_func_t print_func);

  private:
    CWifiClientHandler(CPresenter_ptr presenter);

    void PrintKeyValue(std::string key, std::string value);

    print_func_t _print_func;
};
